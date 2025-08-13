import 'dotenv/config';
import fetch from 'node-fetch';
import cron from 'node-cron';
import { Client, EmbedBuilder, TextChannel } from 'discord.js';

const MCP = process.env.MCP_URL!;
const COUNCIL_CH = process.env.CHN_COUNCIL!;
const LILY_WEBHOOK = process.env.WH_LILYBEAR;
const GH_REPOS = (process.env.NAV_REPOS || '')
  .split(',')
  .map(s => s.trim())
  .filter(Boolean);

async function getMcpStatus(): Promise<string> {
  try {
    const r = await fetch(`${MCP}/ask-gemini`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ prompt: 'Summarize system health in one sentence.' }),
    });
    const j: any = await r.json().catch(() => ({ response: '(no data)' }));
    return (j.response as string) || '(no data)';
  } catch {
    return '(MCP unreachable)';
  }
}

async function getRepoDigest(repo: string): Promise<string> {
  const since = new Date(Date.now() - 24 * 60 * 60 * 1000).toISOString();
  const url = `https://api.github.com/repos/${repo}/commits?since=${encodeURIComponent(since)}&per_page=5`;
  try {
    const r = await fetch(url, {
      headers: { Accept: 'application/vnd.github+json' },
    });
    if (!r.ok) return `• ${repo}: no recent commits`;
    const commits = (await r.json()) as any[];
    if (!commits.length) return `• ${repo}: 0 commits in last 24h`;
    const lines = commits.map(
      c => `• ${repo}@${(c.sha || '').slice(0, 7)} — ${c.commit.message.split('\n')[0]}`,
    );
    return lines.join('\n');
  } catch {
    return `• ${repo}: (error fetching commits)`;
  }
}

export async function sendCouncilReport(client: Client) {
  const ch = client.channels.cache.get(COUNCIL_CH) as TextChannel | undefined;
  const mcp = await getMcpStatus();
  const repoLines = GH_REPOS.length
    ? (await Promise.all(GH_REPOS.map(getRepoDigest))).join('\n')
    : '—';

  const emb = new EmbedBuilder()
    .setTitle('🌙 Nightly Council Report')
    .setDescription('Summary of the last 24h across our realm.')
    .setColor(0x9b59b6)
    .addFields(
      { name: 'System Health (MCP)', value: mcp.slice(0, 1024) || '—' },
      { name: 'Recent Commits', value: repoLines.slice(0, 1024) || '—' },
    )
    .setFooter({ text: 'Reported by Lilybear' })
    .setTimestamp(new Date());

  try {
    if (LILY_WEBHOOK) {
      await fetch(LILY_WEBHOOK, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ embeds: [emb.toJSON()] }),
      });
    } else if (ch) {
      await ch.send({ embeds: [emb] });
    }
  } catch (e) {
    console.error('Council report send error:', e);
  }
}

export function scheduleNightlyCouncilReport(client: Client) {
  cron.schedule(
    '0 8 * * *',
    () => {
      void sendCouncilReport(client);
    },
    { timezone: 'UTC' },
  );
}
