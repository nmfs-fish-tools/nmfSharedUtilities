#pragma once



struct foodSource {
    double dim[50];		// dim[0] = length of link L1, dim[1] = length of link L2, dim[2] = angle THETA
    double objFxValue; 	// Sum of Square Errors (Beta_actual - Beta_theoritical)
    double fitness;
    double trials;
    double prob;
};
