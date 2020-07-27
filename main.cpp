#include <iostream>
#include <boost/program_options.hpp>

#include "distr.h"
#include "management.h"

using namespace std;

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    double mu;
    double si;
    uint32_t minconnections;

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("m",po::value<double>(&mu)->default_value(4.5),"initial µ of lognormal distribution")
            ("s",po::value<double>(&si)->default_value(1),"initial sigma of lognormal distribution")
            ("measures",po::value<uint32_t>(&minconnections)->default_value(8),"amount of required measurements")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    // preparation
    distr sampledGuess(mu,si);

    //distr formulaGuess(mu,si);

    // Main Program
    management mngr;
    uint64_t counter = 0;
    std::string line;
    while(std::getline(std::cin,line)) {
        size_t c1 = line.find(',');
        size_t c2 = line.rfind(',');

        auto timestamp = std::stoull(line.substr(0,c1));
        // we do not need the host id
        // auto hostid = line.substr(c1+1,c2-c1-1);
        auto txid = line.substr(c2+1,line.length()-c2-1);

        auto len = mngr.addTx(timestamp, txid);
        if(len>= minconnections) {
            auto measures = mngr.popTx(txid);

            // adaption process
            normalise(measures);
            updateMuSampled(sampledGuess, measures, 100, 10);
            //updateMuFormula(formulaGuess, measures);

            updateSigma(sampledGuess, measures, sampledGuess.mu);
            //updateSigma(formulaGuess, measures, formulaGuess.mu);

            // output updated values
            //cout << counter << "\t";
            cout << timestamp << ",";
            cout << sampledGuess.mu << "," << sampledGuess.si;
            //cout << "\t|\tµ: " << formulaGuess.mu << " \tsigma: " << formulaGuess.si;
            //cout << "," << txid;
            cout << endl;
            counter ++;
        }
    }

    return 0;
}