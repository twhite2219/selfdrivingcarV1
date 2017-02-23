#include <cstdlib>
#include <iostream>
#include <vector>
 #include <stdlib.h>
#include <unistd.h>
#include <cassert>
#include <stdio.h>
#include <cmath>
 using namespace std;
 
 
 struct Connection 
 {
	 double weight;
		double deltaWeight;
 };
 
 class Neuron;
 
 typedef vector<Neuron> Layer;
 //*******************CLASS NEURON***************
 class Neuron{
	 public:
	Neuron(unsigned numOutputs, unsigned index);
	void setOutputVal(double val){m_outputVal =val;}
	double getOutputVal(void) {return m_outputVal;}
	void feedForward(Layer &prevLayer);
	void calcOutputGradients(double targetVal);
	 void calcHiddenGradients(Layer &nextLayer);
	 void updateInputWeights(Layer  &prevLayer);
	  
	 
	 private:
	 static double eta;//o...1
	 static double alpha; //00.....n
	 double sumDOW(Layer &nextLayer);
	 static double activationFunction(double x);
	 static double activationFunctionDerivative(double x);
	 unsigned m_index;
	double m_outputVal; 
	double m_gradient;
	 vector<Connection> m_outputWeights;
	 static double randomWeight(void){ return rand()/double(RAND_MAX);}
	 
 };
 
 double Neuron::eta=0.15;//learning rate
 double Neuron::alpha=0.5;//momentum multiplier
 
 
 void Neuron::updateInputWeights(Layer &prevLayer)
 {
	 for (unisgned int n =0; n<prevLayer.size();n++){
		 Neuron &neuron =prevLayer[n];
		 double oldDeltaWeight = neuronm_outputWeights[m_Index.deltaWeight;
		 double newDeltaWeight =
		 //indiv input, magnified by gradient n train rate
		 *eta
		 *neuron_getOutputVal()
		*m_gradient
		+alpha
		 *oldDeltaWeight
		 
		 neuron.m_outputWeights[m_index].deltaWeight = newDeltaWeight;
		 neuron.m_outputWeights[m_index].weight += newDeltaWeight;
		 
		 
	 }
 }
 
 double Neuron::sumDOW(Layer &nextLayer){
	 double sum = 0.0;
	 
	 
	 for (unsigned n=0; n<nextLayer.size() -1; n++){
		 sum += m_outputWeights[n] * nextLayer[n].m_gradient;
	 }
	 return sum;
 }
 
 void Neuron::calcOutputGradients(double targetVal){
double delta =targetVal- m_outputVal;
m_gradient= delta * Neuron::activationFunctionDerivative(m_outputVal);	 
 }
 void Neuron::calcHiddenGradients(Layer &nextLayer
 {
	 double dow = sumDOW(nextLayer);
	 m_gradient = dow *  Neuron::activationFunctionDerivative(m_outputVal);	
 }
 
 double Neuron::activationFunction(double x)
 {
	   tanh(x);
 }
 
 
 double Neuron::activationFunctionDerivative(double x);
{
	return 1.0-x*x;
}
 
 void Neuron::feedForward(Layer &prevLayer)
 {
	 double sum = 0.0;
	 
	 for (unsigned n=0; n< prevLayer.size();n++){
		 sum += prevLayer[n].getOutputVal();
		 prevLayer[n].m_outputWeights[m_index].weight;
	 }
	 m_outputVal = Neuron::activationFunction(sum);
		 
 }
 
 Neuron::Neuron(unsigned numOutputs,unsigned index)
 {
	 for (unsigned c=0;c<numOutputs;c++){
		 m_outputWeights.push_back(Connection());
		 m_outputWeights.back().weight = randomWeight();
	 }
	 
	 m_index=index; 
 }
 
 //***********************CLASS NET ********
class Net{
	public:
	Net(vector<unsigned> &topology);
	void feedForward(vector<double> &inputVals);
	void backProp(vector<double> &targetVals)  {};
	void getResults(vector<double> &resultVals) {};
	
	private:
	vector<Layer> m_layers;///m_layers[layernum][neuronNum]
	double m_error;
	double m_recentAverageError;
	double m_recentAverageSmoothingFactor;
};

void Net::getResults(vector<double> &resultVals)
{
resultVals.clear();

for (unsigned n=0;n<m_layers.back().size() -1; n++){
	resultVals.push_back(m_layers.back()[n].getOutputVal());
}	
}

void Net::feedForward(vector<double> &inputVals);
	{
		assert(inputVals.size() == m_layers[0].size()-1);
		
		//assign input values to input neurons
		for (unsigned i=0;i<inputVals.size();i++){
			m_layers[0][i].setOutputVal(inputVals[i]);
		}
		
		
		//forward prop
		for (unsigned layerNum=1;layernum < m_layers.size(); layerNum++){
			Layer &prevLayer =m_layers[layerNum-1];
			for (unsigned n=0; n<m_layers[layerNum].size() -1; n++) {
				m_layers[layerNum][n].feedForward();
			}
		}
	}

void Net::backProp(vector<double> &targetVals)

{
	 //calculate overall net error (rms of output neuron errors,)
	 
	 Layer &outputLayer = m_layers.back();
	 m_error=0.0;
	 
	 for (unsigned n =0; n< outputLayer.size() -1;n++;{
		 double delta =targetVals[n] - outputLayer[n].getOutputVal();
		 m_error += delta * delta;
	 }
	 m_error /= outputLayer.size() -1;
	 m_error = sqrt(m_error);//rms
	 
	 
	 //implement a recent average measurement
	 m_recentAverageError = (m_recentAverageSmoothingFactor +m_error)/(m_recentAverageSmoothingFactor + 1.0)
	 
	 //calc output layer gradients
	 for (unsigned n=0; n<outputLayer.size() -1;n++){
	outputLayer[n].calcOutputGradients(targetVals[n]);	 
	 }
	 //calculate gradients on hidden layers
	 for (unsigned layerNum = m_layers.size() - 2 layerNum > 0; layerNum--){
		 Layer &hiddenLayer = m_layers[layerNum];
		 Layer &nextLayer =m_layers[layerNum +1];
		 
		 for (unsigned n=0; n<hiddenLayer.size(); n++) {
		hiddenLayer[n].calcHiddenGradients(nextLayer);	 
	 }
	 //for all layers from outputs to first hidden layer
	 
	 for (unsigned layerNum = m_layers.size()-1;layerNum > 0; layerNum--){
		 Layer &layer = m_layers[layerNum];
		 Layer &prevLayer =m_layers[layerNum -1];

			for (unsigned n=0; n<layer.size() -1;n++){
				layer[n].updateInputWeights(prevLayer);
			}
		 
	 }
	 //update connection weights
}


Net::Net(vector<unsigned> &topology)
{
	unsigned numLayers =topology.size();
	for (unsigned layerNum =0; layerNum<numLayers; layerNum++){
	
	m_layers.push_back(Layer());//new layer made
	unsigned numOutputs = layerNum = topology.size() - 1 ? 0 : topology[layerNum +1];
	
	for (unsigned neuronNum=0;neuronNum <= topology[layerNum]; neuronNum++){//also add bias neuron to layer
	m_layers.back().push_back(Neuron(numOutputs,neuronNum));
	cout << "made a neuron!!" <<endl;
		}
	}
}

int main(int argc, char **argv)
{	
	vector<unsigned> topology;//eg {3,2,1}
	
	topology.push_back(2500);//layer 1
	topology.push_back(250);//layer 2
	topology.push_back(4);//output layer
	Net CarNN(topology);
	
	vector<double> inputVals;
	CarNN.feedForward(inputVals);
	
	vector<double> targetVals;
	CarNN.backProp(targetVals);
	
	vector<double> resultVals;
	CarNN.getResults(resultVals);
	
	
	
	return 0;
}

