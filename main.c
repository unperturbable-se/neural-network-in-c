#include "neuron.h"

void main(void)
{
    layer* layers=train_dataset();
    image* images=populateImages("test");
    get_accuracy(layers,images);

    while(1)
    {
        printf("\nEnter an index:");
        int i;
        scanf("%d",&i);
        assign_value(&layers[0],images[i].location);
        double* output=load_answer(&layers[0]);
        for(int i=0;i<10;i++)output[i]=(output[i]+1)/2;
        testPrint(images[i].location);
        print_result(output);
        int predicted=argmax(output);
        int actual=images[i].value;
        printf("\n\nPredicted Value:%d, Actual Value:%d\n",predicted,actual);
        printf("\n\n\n\n\n");

    }
}