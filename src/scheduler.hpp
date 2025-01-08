#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <boost/asio.hpp>
#include <functional>


// Класс Scheduler планирует получение данных о курсе валют с определённым интервалом
class Scheduler {
public:
    // io_context нужен для ассинхронных операций
    // interval это интервал времени, через который будет вызываться функция получения данных о курсе валют
    Scheduler(boost::asio::io_context& io_context, std::chrono::seconds interval);

    // Функция принимает лямбда-функцию получения данных о курсе валют, сохраняет её и вызывает schedule_next()
    void start(const std::function<void()>& new_task);

private:
    // Функция, которая планирует выполнение лямбды с заданным интервалом
    // После выполнения лямбды функция вызывает сама себя
    void schedule_next();

    boost::asio::steady_timer timer; // Необходимо хранить для корректной работы таймера
    std::chrono::seconds interval;
    std::function<void()> task; // Лямбда-функция
};


#endif // SCHEDULER_HPP