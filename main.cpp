#include <iostream>
#include <algorithm>
#include <random>

/**
 * Math overflow Q, with a seriously nice answer by Jean-Claude Arbaut
 * https://math.stackexchange.com/questions/4244997/estimating-the-value-of-e-using-a-random-function
 */


/**
 * requires the sizes of the input to be equal
 * @param permutation
 * @param original
 * @return false if any element perm[i] = orig[i] otherwise true
 */
bool is_derangement(const std::vector<int>& permutation, std::vector<int>& original){
    auto first = permutation.begin();
    auto last = permutation.end();
    auto comp_iter = original.begin();
    while(first != last){
        if(*first == *comp_iter){
            return false;
        }
        first++;
        comp_iter++;
    }
    return true;
}
/**
 *
 * @param N
 * @return A vector containing 0..N
 */
std::vector<int> generate_vec(size_t N){
    std::vector<int> v(N);
    auto counter = 0;
    auto incrementor = [&counter](){
        return counter++;
    };
    std::generate(v.begin(), v.end(), incrementor);
    return v;
}

auto VEC_SIZE = 1000;
std::random_device rd;
std::mt19937 g(rd());

double derangement_fraction(int n_samples){
    auto derangements = 0U;
    for(auto i = 0; i < n_samples; i++){
        auto vec = generate_vec(VEC_SIZE);
        auto original = vec;
        std::shuffle(vec.begin(), vec.end(), g);
        if (is_derangement(vec, original)){
            derangements++;
        }
    }
    return n_samples * 1.0 / derangements;
}


/**
 * My std_dev seems to be calculating a too low value. Consistently by about a factor of 5.
 * The derangement method seems to be generating a very non-normal distribution.
 *
 */
using E = double;
template <typename IT>
E std_dev(IT begin, IT end){
    auto N = std::distance(begin, end);
    E average = std::accumulate(begin, end, E()) / N;
    auto sum_term = [average](E init, E value)-> E{
        return init + (value - average)*(value - average);
    };
    E variance = std::accumulate(begin,  end, E(), sum_term);
    return std::sqrt(variance * 1.0 / (N - 1))/std::sqrt(N);
}

int main() {
    auto MAX_COUNT = 1000U; // Makes for precision
    auto SAMPLES = 100U;  // Makes for accuracy
    auto results = std::vector<double>();
    for(auto count = 0U; count < MAX_COUNT; count++ ){
        results.push_back(derangement_fraction(SAMPLES));
    }
    auto avg = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
    auto stddev = std_dev(results.begin(), results.end());
    std::cout << "e = " << avg << " +-" << stddev << "\n";
}
