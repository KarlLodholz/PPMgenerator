#include "ppm.h"

int main() {

    std::string f0,f1,root_name,data0,data1;
    int num_frames,clr_data;
    std::cout<<"file start: ";
    std::cin>>f0;
    std::cout<<"file end: ";
    std::cin>>f1;
    std::cout<<"root name: ";
    std::cin>>root_name;
    std::cout<<"num frames: ";
    std::cin>>num_frames;

    //read files and store their data in arrays
    std::vector<std::vector<int> > frames;
    std::ifstream f;

    for(int i=0;i<num_frames;i++) frames.push_back(std::vector<int>());

    f.open(f0);
    getline(f,data0);
    while(!f.eof()) {
        f>>clr_data;
        frames[0].push_back(clr_data);
    }
    f.close();

    f.open(f1);
    getline(f,data1);
    if(data0 != data1) return 1; //error
    while(!f.eof()) {
        f>>clr_data;
        frames[num_frames-1].push_back(clr_data);
    }
    f.close();

    float diff_inc;
    float color_num;
    for(int i=0; i<frames[0].size(); i++) {
        diff_inc = (frames[0][i] - frames[num_frames-1][i])/(float)num_frames;
        color_num = frames[0][i];
        for(int j=1; j<num_frames-1; j++) {
            color_num += diff_inc;
            frames[j][i] = (int)color_num;
        }
    }

    std::ofstream p;
    for(int i=0; i<num_frames;i+=3) {
        p.open(i+"_"+root_name+".ppm");
        p<<data0;
        for(int j=0; j<frames[0].size(); j++) {
            p<<frames[i][j]+" ";
            p<<frames[i][j+1]+" ";
            p<<frames[i][j+2]+" \t";
        }
        p.close();
    }

    return 0;
}   