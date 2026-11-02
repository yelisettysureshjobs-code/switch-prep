import { Router } from "express";
import bcrypt from "bcrypt";
import { Prisma } from "../../../generated/prisma/client.js";
import { prisma } from "../../db.js";
import { requireAuth, type AuthRequest } from "../../middleware/auth.js";
import { issueTokenPair } from "../auth/auth.service.js";
import { setRefreshCookie } from "../auth/auth.routes.js";

const userRouter = Router();

userRouter.post("/signup", async (req, res) => {
  const { email, password, name } = req.body;

  if (!email || !password) {
    return res.status(400).json({ error: "email and password are required" });
  }

  try {
    const hashedPassword = await bcrypt.hash(password, 10);

    const user = await prisma.user.create({
      data: { email, password: hashedPassword, name },
      select: { id: true, email: true, name: true, createdAt: true },
    });

    const { accessToken, refreshToken } = await issueTokenPair(user.id);
    setRefreshCookie(res, refreshToken);
    return res.status(201).json({ message: "user created", user, accessToken });
  } catch (err) {
    if (
      err instanceof Prisma.PrismaClientKnownRequestError &&
      err.code === "P2002"
    ) {
      return res.status(409).json({ error: "email already registered" });
    }
    return res.status(500).json({ error: "something went wrong" });
  }
});
userRouter.post("/login", async (req, res) => {
  const { email, password } = req.body;
  if (!email || !password) {
    return res.status(400).json({ error: "email and password are required" });
  }
  try {
    const user = await prisma.user.findUnique({
      where: { email },
    });
    if (!user) {
      return res.status(401).json({ error: "invalid email or password" });
    }

    const passwordMatches = await bcrypt.compare(password, user.password);
    if (!passwordMatches) {
      return res.status(401).json({ error: "invalid email or password" });
    }

    const { accessToken, refreshToken } = await issueTokenPair(user.id);
    setRefreshCookie(res, refreshToken);

    const { password: _omitted, ...rest } = user;
    return res.status(200).json({ accessToken, user: rest });
  } catch (err) {
    return res.status(500).json({ error: "something went wrong" });
  }
});

userRouter.get("/me", requireAuth, async (req: AuthRequest, res) => {
  const user = await prisma.user.findUnique({
    where: { id: req.userId },
    select: { id: true, email: true, name: true, createdAt: true },
  });
  if (!user) {
    return res.status(404).json({ error: "user not found" });
  }
  return res.json(user);
});

// userRouter.

export default userRouter;
