/**
 * @file NeuralNetwork.h
 * @brief Fully-connected feedforward neural network class.
 *
 * @defgroup AI Artificial Intelligence Components
 * @brief Core AI classes for neural networks and computations.
 *
 * Responsibilities:
 *  - Maintain an ordered list of Layers
 *  - Perform forward and backward propagation
 *  - Update weights using gradient descent
 *  - Save/load network parameters
 */

#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Matrix.h"
#include "Layer.h"
#include <vector>
#include <functional>
#include <fstream>
#include <string>

/**
 * @class NeuralNetwork
 * @brief Fully-connected feedforward neural network using Layer objects.
 *
 * @ingroup AI
 *
 * Manages multiple Layer objects, each with its own weights, biases, and
 * activation functions. Supports forward pass, backward propagation,
 * training on single samples, and saving/loading weights.
 */
class NeuralNetwork {
public:
    /**
     * @brief Construct a feedforward neural network.
     *
     * @param layerSizes Number of neurons in each layer (including input and output)
     * @param activations Activation functions for each layer (excluding input layer)
     * @param activationDerivatives Derivatives of activation functions for backpropagation
     */
    NeuralNetwork(const std::vector<int>& layerSizes,
                  const std::vector<std::function<double(double)>>& activations,
                  const std::vector<std::function<double(double)>>& activationDerivatives);

    /**
     * @brief Perform a forward pass through all layers.
     * @param input Input vector (Matrix<double>, inputSize × 1)
     * @return Output activations from the final layer
     */
    Matrix<double> forward(const Matrix<double>& input);

    /**
     * @brief Perform backward propagation through all layers and update weights.
     * @param expected Target output vector
     * @param learningRate Learning rate for weight updates
     */
    void backward(const Matrix<double>& expected, double learningRate);

    /**
     * @brief Train the network on a single input-output pair.
     * @param input Input vector
     * @param expected Expected output vector
     * @param learningRate Learning rate
     */
    void train(const Matrix<double>& input, const Matrix<double>& expected, double learningRate);

    /**
     * @brief Predict output from a given input without weight updates.
     * @param input Input vector
     * @return Predicted output vector
     */
    Matrix<double> predict(const Matrix<double>& input);

    /**
     * @brief Save all layer parameters (weights and biases) to a binary file.
     * @param filename File path
     */
    void save(const std::string& filename) const;

    /**
     * @brief Load all layer parameters from a binary file.
     * @param filename File path
     */
    void load(const std::string& filename);

private:
    std::vector<Layer> layers_; ///< Container for layers (encapsulation)
};

#endif // NEURALNETWORK_H