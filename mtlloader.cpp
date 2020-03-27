// #include"mtlloader.h"

// bool MtlLoader::load_mtl(const string mtlpath)
// {
//     ifstream in(mtlpath);
//     //step 1: judge whether the file path is right.
//     if(in.fail())
//     {
//         cout << "The mtl file path does not exist. Please check it.";
//         return false;
//     }
//     //step 2: judge whether the file is empty.
//     streambuf *cinbuf = cin.rdbuf();
//     cin.rdbuf(in.rdbuf());

//     string head;
//     getline(cin, head);
//     if(cin.eof())
//     {
//         cout << "The mtl file is empty. Please check it.\n";
//         return false;
//     }
//     //step 3: judge whether the mtl file format is wrong.
//     istringstream ss(head);
//     string type;
//     ss >> type;
//     if(type != "newmtl")
//     {
//         cout << "The mtl file format is wrong. Please start with \"newmtl\"\n";
//         return false;
//     }

//     //step 4: load mtl file.
//     bool is_end = false;
//     while(!is_end)
//     {
//         if(type == "newmtl")
//         {
//             Material material;
//             ss >> material.mtlname_;
//             //read attributes
//             while(1)
//             {
//                 string content;
//                 getline(cin, content);
//                 if(cin.eof())
//                 {
//                     is_end = true;
//                     break;
//                 }
//                 ss.str(content);
//                 ss.clear();
//                 ss >> type;
//                 if(type == "illum")
//                 {
//                     ss >> material.illum_;
//                 }
//                 else if(type == "Kd")
//                 {
//                     ss >> material.kd_.r_ >> material.kd_.g_ >> material.kd_.b_;
//                     material.diffuse_ = true;
//                 }
//                 else if(type == "Ka")
//                 {
//                     ss >> material.ka_.r_ >> material.ka_.g_ >> material.ka_.b_;
//                     material.self_luminous_ = true;
//                 }
//                 else if(type == "Ks")
//                 {
//                     ss >> material.ks_.r_ >> material.ks_.g_ >> material.ks_.b_;
//                     material.specular_ = true;
//                 }
//                 else if(type == "Tf")
//                 {
//                     ss >> material.tf_.r_ >> material.tf_.g_ >> material.tf_.b_;
//                 }
//                 else if(type == "Tr")//透明度
//                 {
//                     ss >> material.tr_.ratio_;
//                     material.transparent_ = true;
//                 }
//                 else if(type == "Ni")//光密度，折射率
//                 {
//                     ss >> material.ni_.optical_dencity_;
//                     material.transparent_ = true;
//                 }
//                 else if(type == "Ns")
//                 {
//                     ss >> material.ns_.exponent_;
//                     material.specular_ = true;
//                 }
//                 else if(type == "newmtl")
//                 {
//                     break;
//                 }
//             }
//             //add this material here
//             mtl_list.push_back(material);
//         }
//     }
//     cin.rdbuf(cinbuf);
//     return true;
// }

// //void test_mtlloader()
// int main()
// {
//     MtlLoader mtlloader;
//     string mtlname = "/Users/zhangchi/Desktop/CG_RayTracing/diningroom/diningroom.mtl";
//     mtlloader.load_mtl(mtlname);
//     int mtlnum = mtlloader.mtl_list.size();
//     cout << mtlnum << "materials totally.\n";
//     for(int i = 0; i < mtlnum; i++)
//     {
//         cout << i << "): " << mtlloader.mtl_list[i].mtlname_ << endl;
//         cout << "-illum " << mtlloader.mtl_list[i].illum_ << endl;
//         cout << "-Kd " << mtlloader.mtl_list[i].kd_.r_ << ", "
//                         << mtlloader.mtl_list[i].kd_.g_ << ", "
//                         << mtlloader.mtl_list[i].kd_.b_ << endl;
//         cout << "-Ka "<< mtlloader.mtl_list[i].ka_.r_ << ", "
//                         << mtlloader.mtl_list[i].ka_.g_ << ", "
//                         << mtlloader.mtl_list[i].ka_.b_ << endl;
//         cout << "-Tf "<< mtlloader.mtl_list[i].tf_.r_ << ", "
//                         << mtlloader.mtl_list[i].tf_.g_ << ", "
//                         << mtlloader.mtl_list[i].tf_.b_ << endl;
//         cout << "-Ks "<< mtlloader.mtl_list[i].ks_.r_ << ", "
//                         << mtlloader.mtl_list[i].ks_.g_ << ", "
//                         << mtlloader.mtl_list[i].ks_.b_ << endl;
//         cout << "-Ni "<< mtlloader.mtl_list[i].ni_.optical_dencity_ << endl;
//         cout << "-Ns "<< mtlloader.mtl_list[i].ns_.exponent_ << endl;
//         cout << "self_luminous_ " << mtlloader.mtl_list[i].self_luminous_ << endl;
//         cout << "transparent_ " << mtlloader.mtl_list[i].transparent_ << endl;
//         cout << "specular_ " << mtlloader.mtl_list[i].specular_ << endl;
//         cout << "diffuse_ " << mtlloader.mtl_list[i].diffuse_ << endl;
//     }
//     cout << "-------mtl file has loaded successfully!-------\t";
//     return 0;
// }