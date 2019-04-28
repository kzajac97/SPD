#include "scharge.hpp"

auto get_rpq_times(std::vector<process> & processes)
{
    std::vector<int> rtimes, ptimes, qtimes;

    for(auto process : processes)
        { rtimes.push_back(process.get_time()[0]); }

    for(auto process : processes)
        { ptimes.push_back(process.get_time()[1]); }

    for(auto process : processes)
        { qtimes.push_back(process.get_time()[2]); }

    return std::make_tuple(rtimes,ptimes,qtimes);
}

std::vector<process> scharge(std::vector<process> & processes)
{
    std::vector<process> result, schelduable_jobs;
    auto rpq_times = get_rpq_times(processes);
    std::vector<int> rtimes = std::get<0>(rpq_times);
    std::vector<int> ptimes = std::get<1>(rpq_times);
    std::vector<int> qtimes = std::get<2>(rpq_times);
    std::vector<unsigned int> indexes;
    int t = 0;
    bool updated;

    while(!processes.empty() || !schelduable_jobs.empty())
    {
        updated = false;
        for(unsigned int i=0; i < rtimes.size(); i++)
        {
            //std::cout << "P ID " << processes[i].get_id() << "\n";
            //std::cout << "R: " << rtimes[i] << "\n";
            //std::cout << "T: " <<t << "\n";
            if(rtimes[i] <= t)
            {
                updated = true;           
                //schelduable_jobs.push_back(processes[i]);
                //std::cout << "P ID " << processes[i].get_id() << "\n";
                // std::cout << "rtimes: " << rtimes[i] << "\n";
                // std::cout << "T: " <<t << "\n";
                //t += ptimes[i];
                //processes.erase(processes.begin() + i);
                indexes.push_back(i);
                //rtimes.erase(rtimes.begin() + i);
                //ptimes.erase(ptimes.begin() + i);
            }      
        }

        //std::cout << "SJ\n";
        //for(auto x : schelduable_jobs) { std::cout << x.get_id() << " "; } std::cout << "\n"; 
        for(unsigned int j=0; j < schelduable_jobs.size(); ++j)
        {
            unsigned int index = getMaxIndex(schelduable_jobs);
            result.push_back(schelduable_jobs[index]);
            t += schelduable_jobs[index].get_time()[1];
            schelduable_jobs.erase(schelduable_jobs.begin() + index);           
            schelduable_jobs.shrink_to_fit();
        }

        for(unsigned int i=0; i < indexes.size(); ++i)
        {
            schelduable_jobs.push_back(processes[indexes[i]]);
            processes.erase(processes.begin() + indexes[i]);
            rtimes.erase(rtimes.begin() + indexes[i]);
            for(auto & index : indexes) { index--; }
            processes.shrink_to_fit();
            rtimes.shrink_to_fit();
        }

        indexes.clear();
        indexes.shrink_to_fit();

        if(updated) { t++; }

        std::cout << "Result: \n";
        for(auto x : result) { std::cout << x.get_id() << " "; } std::cout << "\n";
    }

    return result;

}

unsigned int getMaxIndex(std::vector<process> & processes)
{
    int max = 0;
    unsigned int index;
    for(unsigned int i=0; i < processes.size(); ++i)
    {
        if(processes[i].get_time().back() > max)
        {
            max = processes[i].get_time().back();
            index = i;
        }
    }

    return index;
}

int rpq_maxspan(std::vector<process> processes)
{
    auto rpq_times = get_rpq_times(processes);
    std::vector<int> rtimes = std::get<0>(rpq_times);
    std::vector<int> ptimes = std::get<1>(rpq_times);
    std::vector<int> qtimes = std::get<2>(rpq_times);
    std::vector<int> maxspan;
    maxspan.resize(rtimes.size());

    maxspan[0] = ptimes[0];

    for(unsigned int i=1; i < ptimes.size(); ++i)
        { maxspan[i] = ptimes[i] + maxspan[i-1]; }

    for(unsigned int j=0; j < rtimes.size(); ++j)
    {
        if(maxspan[j] < rtimes[j]) 
            { maxspan[j] = rtimes[j]; } 
    }

    for(unsigned int k=0; k < qtimes.size(); ++k)
        { maxspan[k] += qtimes[k]; }

    return *std::max_element(maxspan.begin(), maxspan.end());
}
