#include "fetch_currency_data.hpp"
#include "fetch_config_data.hpp"
#include <dpp/dpp.h>
#include <iostream>
#include <thread>


int main() {
    std::string BOT_TOKEN;
    try {
        BOT_TOKEN = fetch_config_data("token");
        std::cout << "Token loaded\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }


    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content | dpp::i_guild_members);

    bot.on_log(dpp::utility::cout_logger());

    //
    // Using cache
    bot.on_message_reaction_remove([&bot](const dpp::message_reaction_remove_t& event) {
        dpp::user* reacting_user = dpp::find_user(event.reacting_user_id);

        if (!reacting_user) {
            bot.log(dpp::ll_info, "User with ID " + std::to_string(event.reacting_user_id) + " was not found");
            return;
        }

        bot.log(dpp::ll_info, reacting_user->format_username() + " removed his reaction.");
    });


    //
    // Using callback functions
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "msgs-get") {
            int64_t limit = std::get<int64_t>(event.get_parameter("quantity"));

            bot.messages_get(event.command.channel_id, 0, 0, 0, limit, [event](const dpp::confirmation_callback_t& callback) {
                if (callback.is_error()) {
                    std::cout << callback.get_error().message << std::endl;
                    return;
                }

                auto messages = callback.get<dpp::message_map>(/*callback.value*/);

                std::string contents;
                for (const auto& x : messages) {
                    contents += x.second.content + '\n';
                }

                event.reply(contents); 

            });

        } else if (event.command.get_command_name() == "channel-create") {
            dpp::channel channel = dpp::channel()
                .set_name("test")
                .set_guild_id(event.command.guild_id);

            bot.channel_create(channel, [&bot, event](const dpp::confirmation_callback_t& callback) -> void {
                if (callback.is_error()) {
                    bot.log(dpp::loglevel::ll_error, callback.get_error().message);
                    return;
                }

                auto channel = callback.get<dpp::channel>();

                dpp::message message = dpp::message("The channel's name is `" + channel.name + "`, ID is `" + std::to_string(channel.id) + "and type is `" + std::to_string(channel.get_type()) + "`.");
                event.reply(message);
            });
        } else if (event.command.get_command_name() == "embed") {
            dpp::embed embed = dpp::embed()
                .set_color(dpp::colors::sti_blue)
                .set_title("Discord bot")
                .set_url("https://dpp.dev/")
                .set_author("KEI3AB", "https://github.com/KEI3AB", "https://avatars.githubusercontent.com/u/191717513?s=400&u=be1cfa463d8ce8d92501bed65666521050cf522c&v=4")
                .set_description("I create it using D++ library")
                .set_thumbnail("https://dpp.dev/DPP-Logo.png")
                .add_field(
                    "The purpose of this bot",
                    "I created this bot to practice my C++ programming skills"
                )
                .add_field(
                    "How does it help",
                    "While you are creating a bot, you add various functionality to it. Because of this, it is constantly necessary to learn something new, for example, previously unknown libraries, methods, functions, etc.",
                    true
                )
                .add_field(
                    "Advice",
                    "If you want improve your hard skills, then create your own project like this. I wish you good luck!",
                    true
                )
                .set_image("https://avatars.githubusercontent.com/u/191717513?s=400&u=be1cfa463d8ce8d92501bed65666521050cf522c&v=4")
                .set_footer(
                    dpp::embed_footer()
                    .set_text("Also you can visit my github")
                    .set_icon("https://avatars.githubusercontent.com/u/191717513?s=400&u=be1cfa463d8ce8d92501bed65666521050cf522c&v=4")
                )
                .set_timestamp(time(0));

            dpp::message msg = dpp::message(event.command.channel_id, embed);

            event.reply(msg);

        // } else if (event.command.get_command_name() == "file") {
        //     dpp::message msg(event.command.channel_id, "Hey there, I've got a new file!");

        //     msg.add_file("file.txt", dpp::utility::read_file("/route/to/your/file.txt"));

        //     event.reply(msg);

        } else if (event.command.get_command_name() == "ping") {
            dpp::message msg(event.command.channel_id, "Pong!");

            event.reply(msg);

        } else if (event.command.get_command_name() == "msg-error") {
            bot.message_get(0, 0, [event](const dpp::confirmation_callback_t& callback) -> void {
                if (callback.is_error()) {
                    event.reply(callback.get_error().message);
                    return;
                }

                auto message = callback.get<dpp::message>();
                event.reply(message);
            });
        
        }
    });
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once <struct register_global_commands>()) {
            dpp::slashcommand msgs_get("msgs-get", "Get message", bot.me.id);

            constexpr int64_t min_val{1};
            constexpr int64_t max_val{100};

            msgs_get.add_option(
                dpp::command_option(dpp::co_integer, "quantity", "Quantity of messages to get. Max - 100.")
                    .set_min_value(min_val)
                    .set_max_value(max_val)
            );

            dpp::slashcommand channel_create("channel-create", "Create a channel", bot.me.id);
            dpp::slashcommand msg_error("msg-error", "Get an error instead of message", bot.me.id);
            dpp::slashcommand embed("embed", "Send a test embed!", bot.me.id);
            dpp::slashcommand file("file", "READ FROM FILE!!1!1!", bot.me.id);
            dpp::slashcommand ping("ping", "Ping Pong!", bot.me.id);

            bot.global_bulk_command_create({ msgs_get, channel_create, msg_error, embed, file, ping });
        }
    });

    std::thread fetcher_thread([]() {
        try {
            fetch_currency_rates();
        } catch (const std::exception& error) {
            std::cerr << "Error in fetch_currency_rates: " << error.what() << std::endl;
        }
    });

    bot.start(dpp::st_wait);

    fetcher_thread.join();

    return 0;
}