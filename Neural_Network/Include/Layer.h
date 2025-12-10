/**
 * @file Layer.h
 * @brief Defines a fully-connected (dense) neural network layer.
 *
 * @defgroup AI Artificial Intelligence Components
 * @brief Core classes for neural network layers and AI computations.
 *
 * Responsibilities:
 *  - Own weights, biases, and activations
 *  - Perform forward and backward propagation
 *
 * Not responsible for:
 *  - Training loops
 *  - Loss computation
 *  - Optimizer logic
 *  - Network serialization (handled at higher level)
 */

#ifndef LAYER_H
#define LAYER_H

#include <functional>
#include <fstream>
#include "Matrix.h"

/**
 * @class Layer
 * @brief Fully-connected (dense) neural network layer.
 *
 * @ingroup AI
 *
 * Provides:
 *  - Forward propagation
 *  - Backward propagation
 *  - Weight & bias storage
 *
 * Activation functions and their derivatives are injected via std::function
 * to follow Dependency Inversion Principle (DIP) and maintain encapsulation.
 */
class Layer {
private:
    int inputSize_;        ///< Number of inputs to the layer
    int neuronCount_;      ///< Number of output neurons

    Matrix<double> W_;     ///< Weight matrix (neurons × inputs)
    Matrix<double> B_;     ///< Bias vector (neurons × 1)
    Matrix<double> Z_;     ///< Weighted sums (W·X + B)
    Matrix<double> A_;     ///< Activations
    Matrix<double> lastInput_; ///< Cached input for backprop

    std::function<double(double)> activationFunc_;   ///< Activation function f(x)
    std::function<double(double)> activationDeriv_;  ///< Derivative f'(x)

public:
    /**
     * @brief Construct a fully-connected layer.
     * @param inputSize Number of input features
     * @param neurons Number of neurons in this layer
     * @param act Activation function f(x)
     * @param actDeriv Derivative of activation function f'(x)
     */
    Layer(int inputSize,
          int neurons,
          std::function<double(double)> act,
          std::function<double(double)> actDeriv);

    /**
     * @brief Perform forward propagation through this layer.
     * @param input Input matrix (size: inputSize × batchSize)
     * @return Reference to activation matrix after forward pass
     */
    const Matrix<double>& forward(const Matrix<double>& input);

    /**
     * @brief Perform backward propagation through this layer.
     * @param dC_dA Gradient of cost w.r.t layer output
     * @param lr Learning rate
     * @return Gradient of cost w.r.t input (to pass to previous layer)
     */
    Matrix<double> backward(const Matrix<double>& dC_dA, double lr);

    /**
     * @brief Get the output activations of the layer.
     * @return Constant reference to the activation matrix
     */
    const Matrix<double>& output() const { return A_; }

    /**
     * @brief Save the layer's weights and biases to a file.
     * @param out Output file stream
     */
    void save(std::ofstream& out) const;

    /**
     * @brief Load the layer's weights and biases from a file.
     * @param in Input file stream
     */
    void load(std::ifstream& in);

    /** @brief Number of inputs to this layer */
    int inputs() const { return inputSize_; }

    /** @brief Number of neurons in this layer */
    int neurons() const { return neuronCount_; }
};

#endif // LAYER_H