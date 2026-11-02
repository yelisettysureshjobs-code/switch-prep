import express from "express";
import cookieParser from "cookie-parser";
import { prisma } from "./db.js";
import userRouter from "./resources/user/user.routes.js";
import authRouter from "./resources/auth/auth.routes.js";

const app = express();
app.use(express.json());
app.use(cookieParser());

app.use("/user", userRouter);
app.use("/auth", authRouter);

app.get("/health", async (_req, res) => {
  try {
    await prisma.$queryRaw`SELECT 1`;
    res.json({ ok: true, db: "connected" });
  } catch (err) {
    res.status(500).json({ ok: false, error: (err as Error).message });
  }
});

const PORT = 3000;
app.listen(PORT, () => {
  console.log(`API running on http://localhost:${PORT}`);
});
