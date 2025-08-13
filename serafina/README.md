# Serafina

TypeScript Discord router for the ShadowFlower Council. Handles slash commands, nightly council reports, and relays whispers to Unity guardians.

## Setup

```bash
pnpm install # or npm install
echo "DISCORD_TOKEN=..." > .env
npm run deploy-commands
npm run build
npm start
```

## Environment

See `.env.example` for required variables.
