import type { Client } from 'discord.js';

export async function startHeartbeat(client: Client) {
  async function beat() {
    try {
      await client.user?.setPresence({
        activities: [{ name: 'whispering across realms' }],
        status: 'online',
      });
    } catch {
      // silence is golden
    }
    setTimeout(beat, 15 * 60 * 1000);
  }
  beat();
}
