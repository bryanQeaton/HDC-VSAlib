#ifndef HDC_HDC_H
#define HDC_HDC_H
#include <matplot/matplot.h>
#include <random>
#include <cmath>
#include <chrono>
#include "cereal/archives/binary.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
inline std::mt19937 gen(std::random_device{}());


/*
todo:
investigate real number HDC's
investigate binary HDC's
investigate bipolar HDC's

real number norm scaled HDC:
random projected HDC's should be normalized to unit vectors

scaling performance based on values and dimensions
try norm(0,1/DIMS)

metrics:
accuracy per ms
total accuracy

similarity metrics:
l1 norm manhattan
*/


/*
Design Paradigm:
    1. all HDC's are unit vectors to prevent exploding magnitudes
    2. bundling is a closed function taking in N vectors as a matrix
        output is a new HDC with associtivity to all vectors N
    3. sum vector can be stored as well to allow for incremental bundling though it is not necessary
        for the standard implementation THIS only applies if we want 1:1 with standard bundling -NOT DONE
    4. incremental training without storing the sum vector may be possible which prevents the worry of
        exploding components by doing additive-subtractive error adjustment with a learning rate based on the amount of error -EXPLORE FIRST

*/
namespace hdc {
    //this was originally a constexpr but I want more control
    inline int DIMS=10000;
    inline void set_dims(const int &dims) {
        DIMS=dims;
    }
    class HDV {
        std::vector<float> vec{};
    public:
        auto begin(){return vec.begin();}
        auto end(){return vec.end();}
        [[nodiscard]] auto begin() const{return vec.begin();}
        [[nodiscard]] auto end() const{return vec.end();}
        auto rbegin(){return vec.rbegin();}
        auto rend(){return vec.rend();}
        [[nodiscard]] auto rbegin() const{return vec.rbegin();}
        [[nodiscard]] auto rend() const{return vec.rend();}
        //================== HDC FUNTIONS
        //fast check if normalized
        [[nodiscard]] bool is_normalized(const float &epsilon=1e-05) const {
            float sum=0.f;
            for (const auto &item:vec) {
                sum+=static_cast<float>(pow(item,2));
                if (sum>1+epsilon*2){return false;}
            }
            if (abs(1.f-sum)<=epsilon){return true;}
            return false;
        }
        //get vector norm
        [[nodiscard]] float vec_norm() const {
            float ret=0.f;
            for (const auto &item:vec) {
                ret+=static_cast<float>(pow(item,2));
            }
            return std::sqrt(ret);
        }
        //normalize vector to unit vector
        void normalize() {
            const float nrm=vec_norm();
            for (auto &item:vec) {item/=nrm;}
        }
        //================== END OF HDC FUNCTIONS

        //================== INITIALIZERS
        //vec is the given vector, n is the number of terms
        explicit HDV(const std::vector<float> &vec):vec(vec) {
            if (static_cast<int>(vec.size())!=DIMS){throw std::runtime_error("Vector dimensions don't match global DIMS!\n");}
            if (!is_normalized()){normalize();}
        }
        //initialize a random hdc
        HDV() {
            vec.resize(DIMS);
            for (auto &item:vec) {
                item=std::normal_distribution<float>(0,1)(gen);
            }
            normalize();
        }
        //================== END OF INITIALIZERS

        //============== OPERATOR OVERLOADS
        float operator[](const int &idx) const {
            return vec[idx];
        }
        float &operator[](const int &idx) {
            return vec[idx];
        }
        //============== END OF OPERATOR OVERLOADS
        //serialize
        template<class Archive>
        void serialize(Archive &archive) {
            archive(vec);
        }
    };
    //bundling operation for defining heirarchies - produces similarities to protos
    inline HDV bundle(const std::vector<HDV> &protos) {
        std::vector res(DIMS,0.f);
        for (const auto &proto:protos) {
            for (int i=0;i<DIMS;i++) {
                res[i]+=proto[i];
            }
        }
        return HDV(res);
    }
    //binding operation for defining relations - produces dissimilarities to protos
    inline HDV bind(const HDV &hdv1,const HDV &hdv2) {
        std::vector res(DIMS,0.f);
        for (int i=0;i<DIMS;i++) {
            res[i]=hdv1[i]*hdv2[i];
        }
        return HDV(res);
    }
    //unbinding operation for recovering relations - z=y*x, x=z*y^-1
    inline HDV unbind(const HDV &binded,const HDV &hdv2) {
        std::vector res(DIMS,0.f);
        for (int i=0;i<DIMS;i++) {
            res[i]=binded[i]*(1/(hdv2[i]+1e-05));
        }
        return HDV(res);
    }
    //permutation operation for defining sequential relationships - produces an hdv shifted by n
    inline HDV perm(HDV hdv1,const int &shift=1) {
        //https://bpb-us-e2.wpmucdn.com/sites.uci.edu/dist/4/4834/files/2025/07/Accelerating-permute-and-n-gram-operations-for-hyperdimensional-learning-in-embedded-systems.pdf
        for (int i=0;i<shift;i++) { //better ways may exist
            std::rotate(hdv1.rbegin(),hdv1.rbegin()+1,hdv1.rend());
        }
        return hdv1;
    }
    namespace similarity {
        inline float cos(const HDV &hdv1,const HDV &hdv2) {
            float dot=0.f;
            for (int i=0;i<DIMS;i++) {
                dot+=hdv1[i]*hdv2[i];
            }
            return dot/(hdv1.vec_norm()*hdv2.vec_norm());
        }
        inline float l1(const HDV &hdv1,const HDV &hdv2) {
            float dist=0.f;
            for (int i=0;i<DIMS;i++) {
                dist+=abs(hdv1[i]-hdv2[i]);
            }
            return dist;
        }
        inline float l2(const HDV &hdv1,const HDV &hdv2) {
            float dist=0.f;
            for (int i=0;i<DIMS;i++) {
                dist+=pow(hdv1[i]-hdv2[i],2);
            }
            return sqrt(dist);
        }
    }
    struct HDVmap {
        struct map_query {
            HDV word_hdv{};
            //serialize
            map_query()=default;
            template<class Archive>
            void serialize(Archive &archive) {
                archive(word_hdv);
            }
        };
        std::unordered_map<std::string,map_query> map{};

        void build(const std::vector<std::vector<std::string>> &data) {
            //to start i need to produce a mapping to random vectors, which will give me a vocabulary to work with
            //then i need to train those vectors to model the relationships in the data
            //if each individual row in the data set is a single instance then no training occurs
            for (auto group:data) {
                for (auto conc:group) {
                    std::cout<<conc;
                }
            }


        }


        /*
        whats the goal of this function

        initialize with saved mapping or new mapping

        .train - takes in dataset to populate mapping and generate semantic data
        .infer - takes in word or set of words and infers similarities to other words in mapping





        training function:
            iterative across dataset:
                take sentence and break it down to word|context pairing
                adjust word towards target word via exponential weighted averaging
                w(t+1)=w(t)*(1-L)+t_w*L

                this is also done subtractively across a subset of words in the vocab that ARE NOT
                part of the current context
                w(t+1)=w(t)*(1-L)-t_w*L

                the ratio of negative updates must be <= the ratio of positive updates
                with positive update ratio of 1 by iterating across the entire dataset
                taking a subset of other words not including the current context should
                follow that rule.




        infer:
            takes 2 words from vocab:
                take words from vocab and return information on semantic similarity




        */
        //serialize
        HDVmap()=default;
        void save(const std::string &FILEPATH) const {
            std::ofstream ofs(FILEPATH,std::ios::binary);
            cereal::BinaryOutputArchive archive(ofs);
            archive(map);
        }
        explicit HDVmap(const std::string &FILEPATH) {
            std::ifstream ifs(FILEPATH,std::ios::binary);
            cereal::BinaryInputArchive archive(ifs);
            archive(map);
        }
        void load(const std::string &FILEPATH) {
            std::ifstream ifs(FILEPATH,std::ios::binary);
            cereal::BinaryInputArchive archive(ifs);
            archive(map);
        }
    };

}








#endif //HDC_HDC_H