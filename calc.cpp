#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <cstdint>
//#include <chrono>
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

// std::vector<int> createMList(int m) {
//     std::vector<int> convertedMList;
//     int start = -(m / 2);
//     int end = m / 2;

//     for (int i = start; i <= end; i++) {
//         convertedMList.push_back(i);
//     }

//     return convertedMList;
// }

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


void calcPossibleConfig(const int l, const int vElectron, const int limit) 
{
    const int m = (2*l+1);
    const int nbConfiguration = fc(m*2)/(fc(vElectron)*fc(m*2-vElectron));
    cout<<nbConfiguration<<endl;

    float totalMicroStates[nbConfiguration][m][2];
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
                //shuffle(tempMicroState, tempMicroState + m, gen);

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
    for(int i=0; i<nbConfiguration; i++)
    {
        for(int j=0; j<m; j++)
        {
            for(int k=0; k<2; k++) {
                cout<<"[";
                cout<<totalMicroStates[i][j][k]<<"]";
            }
            cout<<" ";
        }
        cout<<endl;
    }
    // Ms + Ml values
    int MicroStatesConfigList[nbConfiguration][2];
    //std::vector<int> convertedMList = createMList(m);
    int* convertedMList = createMList(m);
    for (int i=0; i<nbConfiguration; i++)
    {
        float Ms =0;
        int Ml =0;

        for(int j=0; j<m; j++)
        {
            for(int k=0; k<2; k++)
            {
                Ms += totalMicroStates[i][j][k];
                if (totalMicroStates[i][j][k] != 0)
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
    delete[] convertedMList;

}

int main()
{
    //auto start = chrono::high_resolution_clock::now();
    calcPossibleConfig(2,4, 5000);
    //auto stop = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    //cout << duration.count() << endl;
    return 0;
}