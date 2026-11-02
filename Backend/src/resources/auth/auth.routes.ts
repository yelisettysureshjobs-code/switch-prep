import { Router, type Response } from "express";
import {
  rotateRefreshToken,
  revokeFamilyByToken,
  REFRESH_TTL_MS,
} from "./auth.service.js";

const authRouter = Router();

// The refresh token lives in an HttpOnly cookie: scripts can't read it (XSS),
// and the narrow path means the browser only attaches it to /auth requests.
export function setRefreshCookie(res: Response, refreshToken: string) {
  res.cookie("refreshToken", refreshToken, {
    httpOnly: true,
    secure: process.env.NODE_ENV === "production", // localhost is http in dev
    sameSite: "strict",
    path: "/auth",
    maxAge: REFRESH_TTL_MS,
  });
}

authRouter.post("/refresh", async (req, res) => {
  const refreshToken = req.cookies?.refreshToken;
  if (!refreshToken) {
    return res.status(401).json({ error: "missing refresh token" });
  }

  const result = await rotateRefreshToken(refreshToken);
  if ("error" in result) {
    res.clearCookie("refreshToken", { path: "/auth" });
    return res.status(401).json({ error: result.error });
  }

  setRefreshCookie(res, result.refreshToken);
  return res.json({ accessToken: result.accessToken });
});

authRouter.post("/logout", async (req, res) => {
  const refreshToken = req.cookies?.refreshToken;
  if (refreshToken) {
    await revokeFamilyByToken(refreshToken);
  }
  res.clearCookie("refreshToken", { path: "/auth" });
  return res.json({ message: "logged out" });
});

export default authRouter;
