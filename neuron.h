#include "images.h"

typedef struct layer layer;
typedef struct neuron neuron;
//784 16 10
typedef struct neuron
{
    double* weights;
    double bias;
    double error;
} neuron;

typedef struct layer
{
    int population;
    int previousPopulation;
    neuron* neurons;
    double* values;
    layer* prev;
    layer* next;

} layer;

double random_weight(){return ((rand()%1000)-500)/1000.0;}

void init_layer(layer* l,int population,int previousPopulation,layer* next,layer* prev)
{
    l->prev=prev;
    l->next=next;
    l->population=population;
    l->previousPopulation=previousPopulation;
    l->neurons=(neuron*)malloc(sizeof(neuron)*population);
    l->values=(double*)malloc(sizeof(double)*population);
    for(int i=0;i<population;i++)
    {
        (l->neurons[i]).bias=0;
        l->values[i]=0;
        (l->neurons[i]).weights=malloc(sizeof(double)*previousPopulation);
        for(int j=0;j<previousPopulation;j++)(l->neurons[i]).weights[j]=random_weight();
    }
}



layer* init_layers()
{
    layer* layers=(layer*)malloc(sizeof(layer)*3);
    init_layer(&layers[0],784,0,&layers[1],NULL);
    init_layer(&layers[1],16,784,&layers[2],&layers[0]);
    init_layer(&layers[2],10,16,NULL,&layers[1]);
    return layers;

}

void assign_value(layer* l, char* img)
{
    double* values=load_normalized_image(img);
    memcpy(l->values,values,l->population*sizeof(double));
    free(values);
}

double activation_function(double input)
{
    return tanh(input);
}

double* load_answer(layer* head)
{
    double* vals=head->values;
    if(head->next==NULL)return vals;
    double* nextVals=head->next->values;
    int population=head->population;
    for (int i=0;i<head->next->population;i++)
    {
        neuron* n=&(head->next->neurons[i]);
        double* weights=n->weights;
        double bias=n->bias;
        nextVals[i]=bias;
        for(int j=0;j<population;j++)
        {
            nextVals[i]+=weights[j]*vals[j];
        }
        nextVals[i]=activation_function(nextVals[i]);
    }
    return load_answer(head->next);
}

void test()
{
    image* images=populateImages("train");
    layer* layers=init_layers();
    assign_value(&layers[0],images[300].location);
    double* output=load_answer(&layers[0]);
    for(int i=0;i<10;i++)printf("%d:%.3f\n",i,output[i]);
    //for(int i=0;i<60000;i++)free(images[i].location);
    //free(images);
}



