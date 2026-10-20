#include "images.h"

typedef struct layer layer;
typedef struct neuron neuron;
//784 16 10
typedef struct neuron
{
    double* weights;
    double bias;
    double error;
    double delta;
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
        (l->neurons[i]).error=0;
        (l->neurons[i]).bias=0;
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

void propagate_error(layer* tail,double* error) 
{
    if(error!=NULL)for(int i=0;i<tail->population;i++)
    {
        tail->neurons[i].error=error[i];
        tail->neurons[i].delta=error[i]*(1-tail->values[i]*tail->values[i]);
    }

    if(tail->prev==NULL)return;
    layer* prev=tail->prev;
    //error of X0 is equivalent to (error of Y0)*(weight of X0 to Y0) where X0 is a neuron in previous layer 
    // and Y0 is neuron in next layer
    // since multiple neurons are in output layer you do a summation
    for (int i=0;i<prev->population;i++)
    {
        neuron* n=&prev->neurons[i];
        n->error=0;
        for (int j=0;j<tail->population;j++)
        {
            double weight_of_neuron=tail->neurons[j].weights[i];
            double total_error=tail->neurons[j].delta;
            n->error+=weight_of_neuron*total_error;
        }
        n->delta=n->error*(1-prev->values[i]*prev->values[i]);
    }

    propagate_error(prev,NULL);
}

void propagate_weight_and_bias(layer* head,double learning_rate)
{
    if(head->next==NULL)return;
    layer* next=head->next;
    //weight = weight+learning rate*error of output*own value
    for(int i=0;i<next->population;i++)
    {
        neuron* output_neuron= &next->neurons[i];
        double* input_weights=output_neuron->weights;
        for(int j=0;j<head->population;j++)
        {
            double output_error=output_neuron->delta;
            double input_value=head->values[j];
            input_weights[j]+= learning_rate*input_value*output_error;
        }
        output_neuron->bias+=output_neuron->delta*learning_rate;
    }
    propagate_weight_and_bias(head->next,learning_rate);
}

void print_result(double* output)
{
    for(int i=0;i<10;i++)printf("%d:%.3f\n",i,output[i]);
}

layer* train_dataset()
{
    image* images=populateImages("train");
    layer* layers=init_layers();
    double answer[10];
    double error[10];
    int* randomOrder=shuffle(60000);

    for(int i=0;i<60000;i++)
    {
        if(i%200==0)printf("\nTraining (%d/60,000)...",i);
        assign_value(&layers[0],images[randomOrder[i]].location);
        double* output=load_answer(&layers[0]);
        for(int j=0;j<10;j++)answer[j]=-1;
        answer[images[randomOrder[i]].value]=1;
        for(int j=0;j<10;j++)error[j]=answer[j]-output[j];
        propagate_error(&layers[2],error);
        propagate_weight_and_bias(&layers[0],0.01);
    }
    
    for(int i=0;i<60000;i++)free(images[i].location);
    free(images);
    return layers;
}

int argmax(double* output)
{
    double max=output[0];
    int maxIndex=0;
    for(int i=1;i<10;i++)
    {
        if(output[i]>max){max=output[i];maxIndex=i;}
    }
    return maxIndex;
}
void get_accuracy(layer* layers,image* images)
{
    int size=7000;
    int* randomOrder=shuffle(7000);
    int correct=0;
    for(int i=0;i<size;i++)
    {
        assign_value(&layers[0],images[randomOrder[i]].location);
        double* output=load_answer(&layers[0]);
        int answer=argmax(output);
        if(answer==images[randomOrder[i]].value)correct++;
    }
    double accuracy=(100.0*correct)/size;
    printf("\n%.2f percent accuracy\n",accuracy);

}



