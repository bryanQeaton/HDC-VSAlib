# HDC-VSAlib | Header-only High-Dim Vector Representations 

### Todo:
- Trainable Permutation Operator
- Trainable encoding
- Random Projection Encoding
- Iterative Bundling Error Correction
- Bipolar, and Binary HDV's
- Faster Implementation
- Template-based Mapping of all Types

### Features:

- Supports all Standard HDC Operators
(Bundling,Binding,Unbinding,Permutation)
- Word Mapping

> Design paradigm:
> All HDV's are normalized after operations (Like how bundling in Bipolar HDV's is followed by a SGN() operator)
> HDV's can be created by the user for flexibility
> A base mapping tool will also be supplied for formality


## Simple Use Case:

```c++
#include "HDV.h"
#include <iostream>
int main() {
    auto red=hdc::HDV();
    auto green=hdc::HDV();
    auto apple=hdc::HDV();
    auto red_apple=hdc::bind(red,apple);
    auto green_apple=hdc::bind(green,apple);

    //with binding we can produce heirarchical relationships, 
    //but we must unbind 'travelling up' the hierarchy to decode these relationships:

    //binding produces a new HDV proto dissimilar to its constituent protos x*y=Z
    std::cout<<"binding:\n"<<hdc::similarity::cos(red,red_apple)<<" :randomly generated HDV's tend to be orthogonal to eachother.\n";
    //unbinding produces a similar HDV to its constituent proto z*y=x
    std::cout<<hdc::similarity::cos(red,hdc::unbind(red_apple,apple))<<" :red apple - apple = red\n\nbundling:\n";


    //bundling allows for the defining of set relations
    auto apples=hdc::bundle({red_apple,green_apple});
    auto colours=hdc::bundle({red,green});
    std::cout<<hdc::similarity::cos(apples,red_apple)<<" :red apples are in the apples bundle.\n";
    std::cout<<hdc::similarity::cos(apples,red)<<" :the concept of red is not in the apples bundle.\n";

    return 0;
}
```
### Sources:
[Intro] An Introduction to Hyperdimensional Computing for Robotics
[PSPL] Understanding Hyperdimensional Computing for Parallel Single-Pass Learning
[Wiki] Hyperdimensional Computing
[SoHDCaVSA] A Survey on Hypersimensional Computing aka Vector Symbolic Architectures

[Intro]: <https://redwood.berkeley.edu/wp-content/uploads/2021/08/Neubert2019_Article_AnIntroductionToHyperdimension.pdf>
[PSPL]: <https://arxiv.org/pdf/2202.04805>
[Wiki]: <https://en.wikipedia.org/wiki/Hyperdimensional_computing>
[SoHDCaVSA]: <https://arxiv.org/pdf/2111.06077>




Made with <https://dillinger.io/>
