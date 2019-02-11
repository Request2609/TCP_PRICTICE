#include <iostream>
using namespace std;
int main(int argc ,char **argv)
{
    if(argc<2){
        cout<<"./a/out jsonfile"<<endl;
        return 0 ;
    }

    FILE* fp = fopen(argv[1],"r");
    char buf[1024]={0};
    fread(buf,1,sizeof(buf),fp);

    cJSON * root = cJSON_Parse(buf);
    cJSON *subobj = cJSON_GetObjectItem(root,"奔驰");

    //判断对象是否存在
    
    if(subobj){

        //获取子对象
        cJSON * factory = cJSON_GetObjectItem(subobj,"factory");
        CJSON * last = cJSON_GetObjectItem(subobj,"last");
        CJSON * price = cJSON_GetObjectItem(subobj,"price");
        CJSON * sell = cJSON_GetObjectItem(subobj,"sell");
        CJSON * sum = cJSON_GetObjectItem(subobj,"sum");
        CJSON * other = cJSON_GetObjectItem(subobj,"other");

        //打印出value值
        cout<<"奔驰\n";
        cout<<"factory"<<cJSON_Print(factory);
        cout<<"last:"<<cJSON_Print(last);
        //打印数组内容
        if(other->type==cJSON_Array)
        {
            for(int i=0;i<cJSON_GetArraySize(other);++i)
            {
                cJSON *node = cJSON_GetArrayItem(other,i);
                if(node->type==cJSON_String){
                    printf("%s\n",node->valuestring) ;  
                }
                if(node->type==cJSON_Number)
                {
                    printf("%d",node->valueint);
                }
                if(node->type==cJSON_True){
                    printf("%d",noode->valueint);
                }
                if(node->type==cJSON_False)
                {
                    printf("%d\n",node->valueint);
                }
            }
        }
    }
    cJSON_Delete(root);
    fclose(fp);
    return 0;
}

