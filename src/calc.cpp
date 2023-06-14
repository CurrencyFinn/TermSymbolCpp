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
        cout<<endl;
    }
}


float** calcPossibleConfig(const int l, const int vElectron, const int limit) 
{
    const int m = (2*l+1);
    const int nbConfiguration = fc(m*2)/(fc(vElectron)*fc(m*2-vElectron));
    cout<<nbConfiguration<<endl;
    float** totalMicroStates = new float*[nbConfiguration];
    for (int i = 0; i < nbConfiguration; i++) {
        totalMicroStates[i] = new float[m];
    }
    
    int i =0;
    int iCount=0;
    int i_;
    while (i< nbConfiguration)
    {
        int pluggedInElectrons = 0;
        float specificMicroState[m];
        for (int j=0; j<m; j++){
            int k =0;
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
                            specificMicroState[j] = 2.0f; // nomeclature of 2 electrons could rebrand it later
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
        int i__ = i+1;
        for(int p=0; p<i__; p++){
            int resultCounter = 0;
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
                    for (int j = 0; j < m; j++) {
                            totalMicroStates[i][j] = specificMicroState[j];
                    }
                    i++;
                }
                continue;
            }
        }
        if (i_ == i)
        {
            iCount++;
        }
        else
        {
            iCount = 0;
            i_ = i;
        }
        if (iCount >= limit) {
            float randomMicroState;
            while (i<nbConfiguration) 
            {   
                int inputInteger = (rand() % i);
                float tempMicroState[m];
                for(int j = 0; j<m;j++)
                {
                        tempMicroState[j] = totalMicroStates[inputInteger][j];
                }
                random_device rd;
                mt19937 gen(rd());
                for (int q = m - 1; q > 0; --q) {
                    std::uniform_int_distribution<int> distribution(0, q);
                    int v = distribution(gen);
                    std::swap(tempMicroState[q], tempMicroState[v]);
                }
                int i__ = i+1;
                for(int p=0; p<i__; p++){
                    int resultCounter = 0;
                    for (int j = 0; j < m; j++) {
                            if (totalMicroStates[p][j] == tempMicroState[j] && tempMicroState[j] != 0) {
                                resultCounter++;
                        }
                    }
                    if (resultCounter == vElectron) { 
                        p=i__;
                        break;
                    }
                    else {
                        if (p==i)
                        {
                            for (int j = 0; j < m; j++) {
                                    totalMicroStates[i][j] = tempMicroState[j];
                            }
                            i++;
                        }
                        continue;
                    }
                }
            }
            break;
        }
        continue;
    }
    visMicrostates(totalMicroStates, nbConfiguration, m);
    // Ms + Ml values
    float MicroStatesConfigList[nbConfiguration][2];
    int* convertedMList = createMList(m);
    for (int i=0; i<nbConfiguration; i++)
    {
        float Ms =0;
        int Ml =0;

        for(int j=0; j<m; j++)
        {
                if(totalMicroStates[i][j] == 2){
                    Ml += (2*convertedMList[j]);
                }
                else {
                    Ms += totalMicroStates[i][j];
                    if (totalMicroStates[i][j] != 0)
                    {
                        Ml += convertedMList[j];
                    }
                }
        }
        MicroStatesConfigList[i][0] = Ms;
        MicroStatesConfigList[i][1] = Ml;
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
    deleteMicroStates(totalMicroStates, nbConfiguration);
    //return totalMicroStates;
    return 0;
}

int main()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    calcPossibleConfig(3,7, 10000);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Elapsed time: " << elapsedTime << " ms" << std::endl;
    return 0;
}