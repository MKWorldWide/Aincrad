import 'dotenv/config';
import { REST, Routes, SlashCommandBuilder } from 'discord.js';

const TOKEN = process.env.DISCORD_TOKEN!;
const GUILD_ID = process.env.GUILD_ID!;
const CLIENT_ID = process.env.OWNER_ID!; // using owner id as app id placeholder

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

async function main() {
  await rest.put(Routes.applicationGuildCommands(CLIENT_ID, GUILD_ID), {
    body: commands,
  });
  console.log('Slash commands deployed');
}

void main();
