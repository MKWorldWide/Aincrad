import 'dotenv/config';
import {
  Client,
  GatewayIntentBits,
  Interaction,
  REST,
  Routes,
  SlashCommandBuilder,
} from 'discord.js';
import { scheduleNightlyCouncilReport, sendCouncilReport } from './nightlyReport.js';
import { startHeartbeat } from './uptime.js';
import fetch from 'node-fetch';

const TOKEN = process.env.DISCORD_TOKEN!;
const GUILD_ID = process.env.GUILD_ID!;
const CLIENT_ID = process.env.OWNER_ID!; // using owner id as app id placeholder
const GUARDIAN_WEBHOOK = process.env.GUARDIAN_WEBHOOK;

const client = new Client({ intents: [GatewayIntentBits.Guilds] });

async function registerCommands() {
  const commands = [
    new SlashCommandBuilder()
      .setName('council')
      .setDescription('Council utilities')
      .addSubcommandGroup(group =>
        group
          .setName('report')
          .setDescription('Council reports')
          .addSubcommand(sub =>
            sub.setName('now').setDescription('Send council report immediately'),
          ),
      ),
  ].map(c => c.toJSON());

  const rest = new REST({ version: '10' }).setToken(TOKEN);
  await rest.put(Routes.applicationGuildCommands(CLIENT_ID, GUILD_ID), {
    body: commands,
  });
}

client.once('ready', () => {
  console.log(`Serafina online as ${client.user?.tag}`);
  startHeartbeat(client);
  scheduleNightlyCouncilReport(client);
});

client.on('interactionCreate', async (interaction: Interaction) => {
  if (!interaction.isChatInputCommand()) return;
  if (interaction.commandName !== 'council') return;
  if (interaction.options.getSubcommandGroup() === 'report' && interaction.options.getSubcommand() === 'now') {
    await interaction.reply({ content: 'Summoning council report...', ephemeral: true });
    await sendCouncilReport(client);
    if (GUARDIAN_WEBHOOK) {
      try {
        await fetch(GUARDIAN_WEBHOOK, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ from: 'Serafina', to: 'Lilybear', message: 'council report triggered' }),
        });
      } catch (e) {
        console.error('Guardian webhook error:', e);
      }
    }
  }
});

void registerCommands().then(() => client.login(TOKEN));
