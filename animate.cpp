#include "ppm.h"

int main() {

    std::string f0,f1,root_name,data0,data1;
    int num_frames,clr_data,x0,x1,y0,y1,int0,int1,ms;
    bool delete_files;
    std::cout<<"file start: ";
    std::cin>>f0;
    std::cout<<"file end: ";
    std::cin>>f1;
    std::cout<<"root name: ";
    std::cin>>root_name;
    std::cout<<"num frames: ";
    std::cin>>num_frames;
    std::cout<<"ms per frame: ";
    std::cin>>ms;
    std::cout<<"delete files? (0:no , 1:yes): ";
    std::cin>>delete_files;

    //read files and store their data in arrays
    std::vector<std::vector<int> > frames;
    std::ifstream f;

    for(int i=0;i<num_frames;i++) frames.push_back(std::vector<int>());

    f.open(f0);
    getline(f,data0);

    //P3 34 43 255
    f.seekg(3);
    f>>x0;
    f>>y0;
    f>>int0;
    for(int i=0;i<x0*y0*3;i++) {
        f>>clr_data;
        frames[0].push_back(clr_data);
    }
    f.close();

    f.open(f1);
    getline(f,data1);
    f.seekg(3);
    f>>x1;
    f>>y1;
    f>>int1;
    if(data0 != data1) return 1; //error
    for(int i=0;i<x0*y0*3;i++) {
        f>>clr_data;
        frames[num_frames-1].push_back(clr_data);
    }
    f.close();

    float diff_inc;
    float color_num;
    for(int i=0; i<frames[0].size(); i++) {
        diff_inc = (frames[0][i] - frames[num_frames-1][i]) / (float)num_frames;
        color_num = frames[0][i];
        for(int j=1; j<num_frames-1; j++) {
            color_num -= diff_inc;
            frames[j].push_back((int)color_num);
        }
    }

    std::ofstream p;
    std::system(("mkdir "+root_name).c_str());
    for(int i=0; i<num_frames;i++) {
        p.open(root_name+"/"+root_name+"_"+std::to_string(i)+".ppm");
        p<<data0<<"\n";
        for(int j=0; j<frames[0].size(); j+=3) {
            p<<frames[i][j]<<' ';
            p<<frames[i][j+1]<<' ';
            p<<frames[i][j+2]<<"\t";
        }
        p.close();
    }
    //reloop files
    for(int i=1; i<num_frames-1;i++) {
        p.open(root_name+"/"+root_name+"_"+std::to_string(i+num_frames)+".ppm");
        p<<data0<<"\n";
        for(int j=0; j<frames[0].size(); j+=3) {
            p<<frames[num_frames-i-1][j]<<' ';
            p<<frames[num_frames-i-1][j+1]<<' ';
            p<<frames[num_frames-i-1][j+2]<<"\t";
        }
        p.close();
    }


    std::system(("convert -delay "+std::to_string(ms)+" -loop 0 "+root_name+"/*.ppm "+root_name+"/"+root_name+".gif").c_str());

    if(delete_files) std::system(("rm "+root_name+"/*.ppm").c_str());

    return 0;
}   