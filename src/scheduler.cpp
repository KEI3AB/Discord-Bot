#include "scheduler.hpp"

Scheduler::Scheduler(boost::asio::io_context& io_context, std::chrono::seconds interval)
    : timer(io_context), interval(interval) {}

void Scheduler::start(const std::function<void()>& new_task) {
    task = new_task;
    schedule_next();
}

void Scheduler::schedule_next() {
    timer.expires_after(interval);
    timer.async_wait([this](const boost::system::error_code& error) {
        // Если нет ошибки и лямбда-функция задана (не nullptr)
        if (!error && task) {
            task();             // Вызываем лямбда-функцию для сбора данных о курсах валют
            schedule_next();    // Рекурсивно выходим на следующий виток ада
        }
    });

    /* Задача */
    // Добавить обработку ошибки
    // Сделать так, чтобы бот завершал свою работу, после её возникновения
    //
    /* Идеи */
    // если ошибка всё же есть, то кинуть RE
    // Поймать ошибку в fetch_currency_data.cpp (предварительное название) и кинуть дальше RE
    // Поймать ошибку в bot.cpp и завершить работу бота


