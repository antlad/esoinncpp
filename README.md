Hello. This is my implementation of Enhanced self-organizing incremental neural network (ESOINN). 
For theoretical basics I’ve used below article: 

http://cs.nju.edu.cn/rinc/SOINN/e-soinn.pdf 

The project is written in C++. Key features:
- No external dependencies, pure c++(11)
- Multithreading support (OpenMP, speed up about 2.5 times)
- Has functionality to set real label for input data vectors (similar to training with teacher when you have set of data vectors with known label)

----------------------------------------------------------------
Build:

Projects (library and test program) are created with CMake build system. Tested in Linux (Debian), Windows, and Mac OS X. To build test program first build the library, install it and after that build the test program. Test program depends on Boost libraries (system and filesystem).

----------------------------------------------------------------
Test program:

For testing I’ve used Kaggle Digits Recognizer competition. To run the test you need Kaggle digit recognizer data (cvs files). 
You can register on the kaggle.com and get test data here:

https://www.kaggle.com/c/digit-recognizer/data 

Or download it from this link:

https://drive.google.com/folderview?id=0B_t_GIUW2-hIcFVRRktqeDhHWG8&usp=sharing

To run the program you should specify in the first argument the full path to the data folder that contains cvs files: test.cvs and train.cvs .
At first run the test program will convert cvs into binary files to gain more speed for future runs. 
Testing is conducted in three steps: first network will train on data from train.cvs. Then it will run self check and count percentages of failures on train.cvs. At the end network will make prediction on data from  test.cvs, and result file (in Kaggle submission format) will be saved into out.txt.

Update:

New test application with Optical Recognition of Handwritten Digits database (optdigits) (http://www.ics.uci.edu/ ̃mlearn/MLRepository.html) from original paper.

New gui application for debuging step-by-step.

Conclusion:
While testion on optdigits dataset correct recognition ratio is variable from 88% to 97%. It depends on position of random input vectors from this dataset, mean is 92%.(testing parameters are same as in original paper λ = 200, agemax = 50, c1 = 0.001, and c2 = 1.0.. If c2 is set up for example 0.9 mean recognition is 93.5%)

Tests are fully automated. Wihout any manual data manipulation. You are free to check it.

