#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <cstdint>

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
extern "C" __declspec(dllexport) int*** calcPossibleConfig(int l, int vElectron, int limit)
//extern "C" int8_t*** calcPossibleConfig(const int l, const int vElectron, const int limit)
//void calcPossibleConfig(const int l, const int vElectron, const int limit) 
{
    const int m = (2*l+1);
    const int nbConfiguration = fc(m*2)/(fc(vElectron)*fc(m*2-vElectron));

    //int totalMicroStates[nbConfiguration][m][2];
    int*** totalMicroStates = new int**[nbConfiguration];
    for (int i = 0; i < nbConfiguration; i++) {
        totalMicroStates[i] = new int*[m];
        for (int j = 0; j < m; j++) {
            totalMicroStates[i][j] = new int[2];
        }
    }
    int i, iCount=0;
    int i_;
    while (i< nbConfiguration)
    {

        float specificMicroState[m][2];
        int pluggedInElectrons = 0;
        for (int j=0; j<m; j++){
            int k =0;
            while(k<2)
            {
                int inputInteger = (rand() % 3);
                float inputElectron = possibleconfig[inputInteger];
                if (pluggedInElectrons == vElectron) 
                {
                    specificMicroState[j][k] = 0.0f;
                    k++;
                    continue;
                }
                if(k==1)
                {
                    while(specificMicroState[j][0] == inputElectron){
                        int inputInteger = (rand() % 3);
                        float newInputElectron = possibleconfig[inputInteger];

                        if (newInputElectron != inputElectron) {
                            inputElectron = newInputElectron;
                            break; 
                        }
                        inputElectron = newInputElectron;
                    }
                    if (inputElectron == specificMicroState[j][0]*-1) 
                    {
                            pluggedInElectrons++;
                            specificMicroState[j][0] = 0.5f;
                            specificMicroState[j][1] = -0.5f;
                            k++;
                            continue;
                    }

                    else if (specificMicroState[j][0] == 0.0f && inputElectron != 0.0f) 
                    {
                        pluggedInElectrons++;
                        specificMicroState[j][0] = inputElectron;
                        specificMicroState[j][1] = 0.0f;
                        k++;
                        continue;
                    }
                    else 
                    {
                        if (inputElectron != 0.0f)
                        {
                            pluggedInElectrons++;
                        }
                        specificMicroState[j][1] = inputElectron;
                        k++;
                        continue;
                    }
                }
                else {
                    if (inputElectron != 0.0f)
                    {
                        pluggedInElectrons+=1;
                    }
                    specificMicroState[j][k] = inputElectron;
                    k++;
                    continue;
                }
            }
        }
        // first check if the value is already in the total microstates
        int i__ = i+1;
        for(int p=0; p<i__; p++){
            int resultCounter = 0;
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < 2; k++) {
                    if (totalMicroStates[p][j][k] == specificMicroState[j][k] && specificMicroState[j][k] != 0) {
                        resultCounter++;
                    }
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
                        for (int k = 0; k < 2; k++) {
                            totalMicroStates[i][j][k] = specificMicroState[j][k];
                        }
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

                float tempMicroState[m][2];
                for(int j = 0; j<m;j++)
                {
                    for(int k=0; k<2;k++)
                    {
                        tempMicroState[j][k] = totalMicroStates[inputInteger][j][k];
                    }
                }
                // mix orbitals addition of quick check not giving the same one could be added
                random_device rd;
                mt19937 gen(rd());
                for (int q = m - 1; q > 0; --q) {
                    std::uniform_int_distribution<int> distribution(0, q);
                    int v = distribution(gen);
                    std::swap(tempMicroState[q], tempMicroState[v]);
                }

                // check if new in orbital
                int i__ = i+1;
                for(int p=0; p<i__; p++){
                    int resultCounter = 0;
                    for (int j = 0; j < m; j++) {
                        for (int k = 0; k < 2; k++) {
                            if (totalMicroStates[p][j][k] == tempMicroState[j][k] && tempMicroState[j][k] != 0) {
                                resultCounter++;
                            }
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
                                for (int k = 0; k < 2; k++) {
                                    totalMicroStates[i][j][k] = tempMicroState[j][k];
                                }
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
    return totalMicroStates;
}

extern "C" __declspec(dllexport) int*** deleteArray(int*** array, int l, int vElectron)
{
    const int m = (2*l+1);
    const int nbConfiguration = fc(m*2)/(fc(vElectron)*fc(m*2-vElectron));
    for (int i = 0; i < nbConfiguration; i++) {
        for (int j = 0; j < m; j++) {
            delete[] array[i][j];
        }
        delete[] array[i];
    }
    delete[] array;
    return 0;
}

int main()
{
    return 0;
}