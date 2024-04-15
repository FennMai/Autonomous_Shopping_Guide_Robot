// KalmanFilter.h
#ifndef KALMANFILTER_H
#define KALMANFILTER_H

class KalmanFilter {
public:
    KalmanFilter(double process_noise = 0.01, double measurement_noise = 0.1, double estimate_error = 1.0)
        : q(process_noise), r(measurement_noise), p(estimate_error), k(0), x(0) {}

    double updateEstimate(double measurement) {
        // Prediction update
        p += q;

        // Measurement update
        k = p / (p + r);
        x += k * (measurement - x);
        p *= (1 - k);

        return x;
    }

    void setProcessNoise(double noise) {
        q = noise;
    }

    void setMeasurementNoise(double noise) {
        r = noise;
    }

    void setEstimateError(double error) {
        p = error;
    }

private:
    double q; // process noise covariance
    double r; // measurement noise covariance
    double p; // estimation error covariance
    double k; // kalman gain
    double x; // value
};

#endif
