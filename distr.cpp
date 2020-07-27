//
// Created by moedinger on 02.05.19.
//

#include <algorithm>
#include "distr.h"

// ---------------------- distr methods ---------------------------

distr::distr(double mu, double si) : mu(mu), si(si), dist(mu, si) {}

void distr::update_mu(double change) {
    mu += change;
    dist = std::lognormal_distribution<double>(mu,si);
}

void distr::update_si(double change) {
    si += change;
    dist = std::lognormal_distribution<double>(mu,si);
}

double distr::generate() {
    return dist(generator);
}

// ----------------------- functions ------------------------------

auto sample(distr& dist, uint32_t samplesize, double shift) {
    std::vector<uint64_t> result;
    for(auto i=0; i<samplesize; i+=1) {
        auto gened = dist.generate()+shift;
        auto tmp = static_cast<uint64_t>(gened);
        result.push_back(tmp);
    }
    return result;
}

void normalise(std::vector<uint64_t>& toreduce) {
    std::sort(toreduce.begin(), toreduce.end());
    auto reduce = toreduce[0];
    for(auto &e: toreduce)
        e-=reduce;
}

double error(double si) {
    return -0.207898*si*si+0.083586*si-0.032573;
}

void updateMuSampled(distr &adaptive, std::vector<uint64_t> measured, uint32_t rounds, uint32_t measurements) {
    auto measured_mean = mean(measured);

    std::vector<double> means;
    for (auto i = 0; i < rounds; i += 1) {
        auto v = sample(adaptive, measurements, 0);
        normalise(v);
        means.push_back(mean(v));
    }

    auto sampled_mean = mean(means);
    auto change = (log(measured_mean)-log(sampled_mean)-(error(adaptive.si)));

    adaptive.update_mu(change/20);
}

void updateMuFormula(distr &formula, std::vector<uint64_t> measured) {
    auto mmean = mean(measured);

    // variance
    double var = variance(measured, mmean);

    // skewness
    double c = 0;
    for(auto e: measured) {
        c += (e-mmean)*(e-mmean)*(e-mmean);
    }
    c /= measured.size()-1;
    auto s = sqrt(var);
    c /= s*s*s;

    auto c2 = c*c;
    auto c4 = c2*c2;
    auto sigma = cbrt((2+c2+sqrt(4*c2+c4))/2);
    sigma = sigma+ 1/sigma -1;
    sigma = abs(sqrt(log(sigma)));
    auto mu = log(var/exp(sigma*sigma)-1)-(sigma*sigma)/2;

    auto change = mu-formula.mu-error(formula.si);

    formula.update_mu(change/20);
}

void updateSigma(distr& adaptive, std::vector<uint64_t>& measured, double _mean) {
    std::vector<double> logdata;
    for(auto e: measured) {
        if(e>0)
            logdata.push_back(log(e));
    }
    auto sig = getSigma(logdata, _mean);
    auto change = (sig - adaptive.si)/2000;
    adaptive.update_si(change);
}