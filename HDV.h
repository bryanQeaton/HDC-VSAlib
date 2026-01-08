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
        HDV(const bool &null=false) {
            if (null) {
                vec.resize(DIMS);
                for (auto &item:vec) {
                    item=0.f;
                }
            }
            else {
                vec.resize(DIMS);
                for (auto &item:vec) {
                    item=std::normal_distribution<float>(0,1)(gen);
                }
                normalize();
            }
        }
        //================== END OF INITIALIZERS

        //============== OPERATOR OVERLOADS
        float operator[](const int &idx) const {
            return vec[idx];
        }
        float &operator[](const int &idx) {
            return vec[idx];
        }
        auto operator+(const HDV &hdv) const {
            HDV result=*this;
            for (int i=0;i<DIMS;i++) {
                result.vec[i]+=hdv.vec[i];
            }
            return result;
        }
        auto operator+=(const HDV &hdv) {
            for (int i=0;i<DIMS;i++) {
                this->vec[i]+=hdv.vec[i];
            }
            return this;
        }
        auto operator-(const HDV &hdv) const {
            HDV result=*this;
            for (int i=0;i<DIMS;i++) {
                result.vec[i]-=hdv.vec[i];
            }
            return result;
        }
        auto operator-=(const HDV &hdv) {
            for (int i=0;i<DIMS;i++) {
                this->vec[i]-=hdv.vec[i];
            }
            return this;
        }
        auto operator*(const float &scalar) const {
            HDV result=*this;
            for (auto &item:result.vec) {
                item*=scalar;
            }
            return result;
        }
        auto operator*=(const float &scalar) {
            for (auto &item:this->vec) {
                item*=scalar;
            }
            return this;
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
        struct word_query {
            HDV word_hdv{};
            operator HDV() const {
                return word_hdv;
            }
            //serialize
            word_query()=default;
            template<class Archive>
            void serialize(Archive &archive) {
                archive(word_hdv);
            }
        };
        std::unordered_map<std::string,word_query> word_map{};
        void build(const std::vector<std::vector<std::string>> &data,const float &learning_rate=.1,const float &pos_neg_ratio=.25f) {
            auto data_context=HDV(true);
            for (const auto &group:data) {
                for (int i=0;i<static_cast<int>(group.size());i++) {
                    if (!word_map.contains(group[i])) { //generate random HDV's for each word
                        word_map[group[i]].word_hdv=HDV();
                    }
                    data_context+=word_map[group[i]].word_hdv;
                }
            }
            for (const auto &group:data) {
                if (group.size()<=1) { //no training on single word groupings
                    continue;
                }
                auto group_context=HDV(true);
                for (int i=0;i<static_cast<int>(group.size());i++) {
                    group_context+=word_map[group[i]].word_hdv; //generate group context based on words
                }
                for (int i=0;i<static_cast<int>(group.size());i++) {
                    HDV positive_target=group_context-word_map[group[i]].word_hdv; //target is the group context minus the current word
                    HDV negative_target=data_context-group_context;
                    positive_target.normalize();
                    negative_target.normalize();
                    //additive sampling is taking the word hdv and updating it towards the target
                    word_map[group[i]].word_hdv=word_map[group[i]].word_hdv*(1-learning_rate)+positive_target*learning_rate;
                    //negative sampling is taking the word hdv and updating it away from the bundle of context
                    //across the entire dataset minus the current context
                    word_map[group[i]].word_hdv=word_map[group[i]].word_hdv*(1-learning_rate*pos_neg_ratio)-negative_target*learning_rate*pos_neg_ratio;
                }





                //subtractive sampling should push single words away from other words or contexts away from other contexts
                //random sampling with noise could be possible to prevent extra overhead
                //make a set of pairs across entire dataset
                //delete pairs that share context
                //push all pairs away from eachother equally


                

                
            }


        }


        /*
        whats the goal of this function

        initialize with saved mapping or new mapping

        .train - takes in dataset to populate mapping and generate semantic data
        .infer - takes in word or set of words and infers similarities to other words in mapping
        */
        //serialize
        HDVmap()=default;
        void save(const std::string &FILEPATH) const {
            std::ofstream ofs(FILEPATH,std::ios::binary);
            cereal::BinaryOutputArchive archive(ofs);
            archive(word_map);
        }
        explicit HDVmap(const std::string &FILEPATH) {
            std::ifstream ifs(FILEPATH,std::ios::binary);
            cereal::BinaryInputArchive archive(ifs);
            archive(word_map);
        }
        void load(const std::string &FILEPATH) {
            std::ifstream ifs(FILEPATH,std::ios::binary);
            cereal::BinaryInputArchive archive(ifs);
            archive(word_map);
        }
    };

}








#endif //HDC_HDC_H