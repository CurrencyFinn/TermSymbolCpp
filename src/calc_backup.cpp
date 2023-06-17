#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <cstdint>
#include <chrono>
using namespace std;
double possibleconfig[3] = {0.0f,0.5f,-0.5f};

unsigned long long fc(unsigned int n)
{
    uint64_t res=1;
    for (int z = 2; z <= n; z++)
    {
        res *= z;
    }
    return res;
}

int* createMList(int m) {
    int* convertedMList = new int[m];
    int start = -(m / 2);
    int end = m / 2;
    int index = 0;

    for (int i = start; i <= end; i++) {
        convertedMList[index++] = i;
    }

    return convertedMList;
}

void deleteMicroStates(float** microstates, int nbConfig) {
    for (int i = 0; i < nbConfig; i++) {
            delete[] microstates[i];
    }
    delete[] microstates;
}

void visMicrostates(float** microstates, int nbConfig, int M) {
    for(int i=0; i<nbConfig; i++)
    {
        for(int j=0; j<M; j++)
        {
                cout<<"[";
                cout<<microstates[i][j]<<"]";
            cout<<" ";
        }
        cout<<"\n";
    }
}


void calcPossibleConfig(const int l, const int vElectron) 
{
    const int m = (2*l+1);
    const int nbConfiguration = fc(m*2)/(fc(vElectron)*fc(m*2-vElectron));
    cout<<nbConfiguration<<endl;

    int i_;
    int i__;
    float Ms;
    int Ml;
    int resultCounter;
    float** totalMicroStates = new float*[nbConfiguration];
    float MicroStatesConfigList[nbConfiguration][2];
    int* convertedMList = createMList(m);
    for (int i = 0; i < nbConfiguration; i++) {
        totalMicroStates[i] = new float[m];
    }
    int k;

    int i =0;
    while (i< nbConfiguration)
    {
        int pluggedInElectrons = 0;
        float* specificMicroState = new float[m];
        for (int j=0; j<m; j++){
            k =0;
            while(k<2)
            {
                int inputInteger = (rand() % 3);
                float inputElectron = possibleconfig[inputInteger];
                if (pluggedInElectrons == vElectron) 
                {
                    specificMicroState[j] = 0.0f;
                    k++;
                    break;
                }
                if(k==1)
                {
                    while(specificMicroState[j] == inputElectron){ // pauli exclusion
                        int inputInteger = (rand() % 3);
                        float newInputElectron = possibleconfig[inputInteger];

                        if (newInputElectron != inputElectron) {
                            inputElectron = newInputElectron;
                            break; 
                        }
                        inputElectron = newInputElectron;
                    }
                    if (inputElectron == specificMicroState[j]*-1) 
                    {
                            specificMicroState[j] = 2.0f; // nomeclature of 2 electrons
                            k++;
                            break;
                    }

                    else if (specificMicroState[j] == 0.0f && inputElectron != 0.0f) 
                    {
                        specificMicroState[j] = inputElectron;
                        k++;
                        continue;
                    }
                    else 
                    {
                        specificMicroState[j] = inputElectron;
                        k++;
                        continue;
                    }
                }
                else {
                    specificMicroState[j] = inputElectron;
                    k++;
                    continue;
                }
            }
            if(specificMicroState[j] != 0.0f) {
                if(specificMicroState[j] == 2)
                {
                    pluggedInElectrons++;
                }
                pluggedInElectrons++;
            }
        }
        if (pluggedInElectrons != vElectron)
        {
            continue;
        }
        i__ = i+1;
        for(int p=0; p<i__; p++){
            resultCounter = 0;
            for (int j = 0; j < m; j++) {
                if (totalMicroStates[p][j] == specificMicroState[j]) { 
                    resultCounter++;
                }
            }
            if (resultCounter == m) { 
                p=i__;
                break;
            }
            else {
                if (p==i)
                {
                    sort(specificMicroState, specificMicroState + m);
                    Ms = 0, Ml =0;
                    for (int j = 0; j < m; j++) {
                            totalMicroStates[i][j] = specificMicroState[j];
                            if(specificMicroState[j] != 0){
                                if (specificMicroState[j] == 2){
                                    Ml += 2*convertedMList[j];
                                }
                                else{
                                    Ms += specificMicroState[j];
                                    Ml += convertedMList[j];
                                }
                            }
                    }
                    MicroStatesConfigList[i][0] = Ms;
                    MicroStatesConfigList[i][1] = Ml;
                    i++;
                    Ml = 0;
                    while (next_permutation(specificMicroState, specificMicroState + m)) {
                        for (int j = 0; j < m; j++) {
                            totalMicroStates[i][j] = specificMicroState[j];
                            if(specificMicroState[j] != 0){
                                if (specificMicroState[j] == 2){
                                    Ml += 2*convertedMList[j];
                                }
                                else{
                                    Ml += convertedMList[j];
                                }
                            }
                        }
                        MicroStatesConfigList[i][0] = Ms;
                        MicroStatesConfigList[i][1] = Ml;
                        i++;
                        Ml = 0;
                    }
                }
                continue;
            }
        }
        continue;
    }
    for(int i=0; i<nbConfiguration; i++)
    {
        for(int j=0; j<2; j++)
        {
            cout<<"[";
            cout<<MicroStatesConfigList[i][j]<<"]";
        }
        cout<<" ";
    }
    cout<<endl;
    delete[] convertedMList;
    //visMicrostates(totalMicroStates,nbConfiguration,m);
    deleteMicroStates(totalMicroStates, nbConfiguration);
}

int main()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    calcPossibleConfig(3,7);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Elapsed time: " << elapsedTime << " ms" << std::endl;
    return 0;
}