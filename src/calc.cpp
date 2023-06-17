#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>
using  std::cout, std::endl;

double possibleconfig[3] = {2,0.5f,-0.5f};

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

void writeArrayToCSV(float** array, int numRows, int numCols, const std::string& filename) {
    std::ofstream file(filename);

    if (file.is_open()) {
        for (int i = 0; i < numRows; ++i) {
            file << array[i][0]; // Write the first index value

            for (int j = 1; j < numCols; ++j) {
                file << ',' << array[i][j]; // Write the values in the row separated by commas
            }

            file << '\n'; // Move to the next line
        }

        file.close();
        std::cout << "Array successfully written to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
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
    int* convertedMList = createMList(m);
    //float MicroStatesConfigList[nbConfiguration][2];
    float** MicroStatesConfigList = new float*[nbConfiguration];
    for (int i = 0; i < nbConfiguration; i++) {
        MicroStatesConfigList[i] = new float[2];
    }
    float** totalMicroStates = new float*[nbConfiguration];
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


            if (pluggedInElectrons == vElectron) 
            {
                specificMicroState[j] = 0.0f;
                k++;
                continue;
            }
            else
            {
                int inputInteger = (rand() % 3);
                float inputElectron = possibleconfig[inputInteger];
                specificMicroState[j] = inputElectron;
                if(specificMicroState[j] == 2)
                {
                    pluggedInElectrons++;
                }
                pluggedInElectrons++;
                continue;
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
                    std::sort(specificMicroState, specificMicroState + m);
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
                    while (std::next_permutation(specificMicroState, specificMicroState + m)) {
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
    // for(int i=0; i<nbConfiguration; i++)
    // {
    //     for(int j=0; j<2; j++)
    //     {
    //         cout<<"[";
    //         cout<<MicroStatesConfigList[i][j]<<"]";
    //     }
    //     cout<<" ";
    // }
    //cout<<endl;
    delete[] convertedMList;
    //visMicrostates(totalMicroStates,nbConfiguration,m);
    writeArrayToCSV(MicroStatesConfigList, nbConfiguration, 2, "output.csv");
    deleteMicroStates(MicroStatesConfigList, nbConfiguration);
    deleteMicroStates(totalMicroStates, nbConfiguration);
}

int main()
{
    calcPossibleConfig(3,7);
    return 0;
}