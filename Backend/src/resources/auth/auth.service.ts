import crypto from "node:crypto";
import jwt from "jsonwebtoken";
import { prisma } from "../../db.js";

export const ACCESS_TTL = "15m";
export const REFRESH_TTL_MS = 7 * 24 * 3600 * 1000; // 7 days

// sha256 is fine for refresh tokens (unlike passwords): 32 random bytes
// can't be guessed, so a leaked hash can't be turned back into a token.
const sha256 = (value: string) =>
  crypto.createHash("sha256").update(value).digest("hex");

export function signAccessToken(userId: number) {
  return jwt.sign({ userId }, process.env.JWT_SECRET!, {
    expiresIn: ACCESS_TTL,
  });
}

// Issues an access token + opaque refresh token. Only the sha256 hash of the
// refresh token is stored; familyId ties together every token descended from
// one login so the whole chain can be revoked at once.
export async function issueTokenPair(userId: number, familyId?: string) {
  const refreshToken = crypto.randomBytes(32).toString("hex");

  await prisma.refreshToken.create({
    data: {
      userId,
      familyId: familyId ?? crypto.randomUUID(),
      tokenHash: sha256(refreshToken),
      expiresAt: new Date(Date.now() + REFRESH_TTL_MS),
    },
  });

  return { accessToken: signAccessToken(userId), refreshToken };
}

// Rotation: a refresh token works exactly once. Seeing a used token again
// means it leaked (or a client raced), so the whole family is revoked.
export async function rotateRefreshToken(refreshToken: string) {
  const row = await prisma.refreshToken.findUnique({
    where: { tokenHash: sha256(refreshToken) },
  });

  if (!row) return { error: "unknown token" as const };
  if (row.expiresAt <= new Date()) return { error: "expired" as const };
  if (row.usedAt) {
    await prisma.refreshToken.deleteMany({ where: { familyId: row.familyId } });
    return { error: "reuse detected: family revoked" as const };
  }

  await prisma.refreshToken.update({
    where: { id: row.id },
    data: { usedAt: new Date() },
  });

  return issueTokenPair(row.userId, row.familyId);
}

export async function revokeFamilyByToken(refreshToken: string) {
  const row = await prisma.refreshToken.findUnique({
    where: { tokenHash: sha256(refreshToken) },
  });
  if (row) {
    await prisma.refreshToken.deleteMany({ where: { familyId: row.familyId } });
  }
}
