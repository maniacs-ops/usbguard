/*************************************************************************/
/* spdlog - an extremely fast and easy to use c++11 logging library.     */
/* Copyright (c) 2014 Gabi Melman.                                       */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

//
// bench.cpp : spdlog benchmarks
//
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include "spdlog/spdlog.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/file_sinks.h"
#include "spdlog/sinks/null_sink.h"
#include "utils.h"


using namespace std;
using namespace std::chrono;
using namespace spdlog;
using namespace spdlog::sinks;
using namespace utils;


void bench(int howmany, std::shared_ptr<spdlog::logger> log);
void bench_mt(int howmany, std::shared_ptr<spdlog::logger> log, int thread_count);

int main(int argc, char* argv[])
{

    int queue_size = 1048576;
    int howmany = 1000000;
    int threads = 10;
    bool auto_flush = false;
    int file_size = 30 * 1024 * 1024;
    int rotating_files = 5;

    try
    {

        if(argc > 1)
            howmany = atoi(argv[1]);
        if (argc > 2)
            threads =   atoi(argv[2]);
        if (argc > 3)
            queue_size = atoi(argv[3]);


        cout << "*******************************************************************************\n";
        cout << "Single thread, " << format(howmany)  << " iterations, auto flush=" << auto_flush << endl;
        cout << "*******************************************************************************\n";

        auto rotating_st = spdlog::rotating_logger_st("rotating_st", "logs/rotating_st", file_size, rotating_files, auto_flush);
        bench(howmany, rotating_st);
        auto daily_st = spdlog::daily_logger_st("daily_st", "logs/daily_st", auto_flush);
        bench(howmany, daily_st);
        bench(howmany, spdlog::create<null_sink_st>("null_st"));

        cout << "\n*******************************************************************************\n";
        cout << threads << " threads sharing same logger, " << format(howmany)  << " iterations, auto_flush=" << auto_flush << endl;
        cout << "*******************************************************************************\n";

        auto rotating_mt = spdlog::rotating_logger_mt("rotating_mt", "logs/rotating_mt", file_size, rotating_files, auto_flush);
        bench_mt(howmany, rotating_mt, threads);


        auto daily_mt = spdlog::daily_logger_mt("daily_mt", "logs/daily_mt", auto_flush);
        bench_mt(howmany, daily_mt, threads);
        bench(howmany, spdlog::create<null_sink_st>("null_mt"));

        cout << "\n*******************************************************************************\n";
        cout << "async logging.. " << threads << " threads sharing same logger, " << format(howmany) << " iterations, auto_flush=" << auto_flush << endl;
        cout << "*******************************************************************************\n";


        spdlog::set_async_mode(queue_size);

        for(int i = 0; i < 3; ++i)
        {
            auto as = spdlog::daily_logger_st("as", "logs/daily_async", auto_flush);
            bench_mt(howmany, as, threads);
            spdlog::drop("as");
        }
    }
    catch (std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        perror("Last error");
    }
    return 0;
}


void bench(int howmany, std::shared_ptr<spdlog::logger> log)
{
    cout << log->name() << "...\t\t" << flush;
    auto start = system_clock::now();
    for (auto i = 0; i < howmany; ++i)
    {
        log->info("Hello logger: msg number {}", i);
    }


    auto delta = system_clock::now() - start;
    auto delta_d = duration_cast<duration<double>> (delta).count();
    cout << format(int(howmany / delta_d)) << "/sec" << endl;
}


void bench_mt(int howmany, std::shared_ptr<spdlog::logger> log, int thread_count)
{

    cout << log->name() << "...\t\t" << flush;
    std::atomic<int > msg_counter {0};
    vector<thread> threads;
    auto start = system_clock::now();
    for (int t = 0; t < thread_count; ++t)
    {
        threads.push_back(std::thread([&]()
        {
            for(;;)
            {
                int counter = ++msg_counter;
                if (counter > howmany) break;
                log->info("Hello logger: msg number {}", counter);
            }
        }));
    }


    for(auto &t:threads)
    {
        t.join();
    };


    auto delta = system_clock::now() - start;
    auto delta_d = duration_cast<duration<double>> (delta).count();
    cout << format(int(howmany / delta_d)) << "/sec" << endl;
}