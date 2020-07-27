//
// Created by moedinger on 02.05.19.
//

#include <random>
#include <vector>

#ifndef BTCMON_DISTR_H
#define BTCMON_DISTR_H

// ---------------------- distr class ---------------------------

class distr {
private:
    std::default_random_engine generator;
    std::lognormal_distribution<double> dist;

public:
    double mu;
    double si;

    distr(double mu, double si);
    void update_mu(double change);
    void update_si(double change);
    double generate();
};

// ----------------------- functions ------------------------------

/**
 * sample the distribution dist with engine gen for samplesize times and shift the results by shift, return measurements as integer
 * @param dist Distribution to sample
 * @param samplesize Count of samples
 * @param shift gamma shift of samples
 * @return
 */
auto sample(distr& dist, uint32_t samplesize, double shift);

/**
 * Normalise a vector by its smallest element
 * @param toreduce
 */
void normalise(std::vector<uint64_t>& toreduce);

/**
 * Precomputed error formula dependent on sigma
 * @param sigma of the distribution
 */
double error(double si);

/**
 * Updates the distribution based on the sample scheme
 *
 * @param adaptive Updated distribution
 * @param measured Measurements used to update the distribution
 * @param rounds Count of rounds to use for sampling
 * @param measurements Count of samples per round
 */
void updateMuSampled(distr &adaptive, std::vector<uint64_t> measured, uint32_t rounds, uint32_t measurements);

/**
 * Updates the distribution based on a formula and moments computation
 *
 * @param formula Updated distribution
 * @param measured Measurements used to update the distribution
 * @param rounds Count of rounds to use for sampling
 * @param measurements Count of samples per round
 */
void updateMuFormula(distr &formula, std::vector<uint64_t> measured);

/**
 * Updates sigma value based on data
 * @param adaptive Distribution to update
 * @param measures Vector of measurements
 * @param _mean Mean to use in sigma computation
 */
void updateSigma(distr& adaptive, std::vector<uint64_t>& measures, double _mean);
// ----------------------- templates ------------------------------

/**
 * Compute mean of given sample
 * @param samples
 * @return
 */
template<typename T>
double mean(T& samples) {
    double tmp = 0;
    for(auto i:samples){
        tmp += i;
    }
    return tmp/samples.size();
}

template<typename T>
double variance(T& measured, double mean) {
    double var = 0;
    for(auto e: measured) {
        var += (e-mean)*(e-mean);
    }
    var /= measured.size();

    return var;
}

template<typename T>
double getSigma(std::vector<T>& data, double m) {
    return sqrt(variance(data, m));
}

#endif //BTCMON_DISTR_H
