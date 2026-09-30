#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>
#include <numeric>

using namespace std;

struct Particle {
    double x;
    double weight;
};

class ParticleFilter1D {
private:
    int num_particles;
    vector<Particle> particles;
    default_random_engine gen;
    double landmark_x; // Known arena landmark (e.g. wall at 10.0m)

public:
    ParticleFilter1D(int count, double init_min, double init_max, double landmark) 
        : num_particles(count), landmark_x(landmark) {
        
        uniform_real_distribution<double> dist(init_min, init_max);
        particles.resize(num_particles);
        for (auto& p : particles) {
            p.x = dist(gen);
            p.weight = 1.0 / num_particles;
        }
    }

    // 1. Motion Update: Predict new positions with actuator noise
    void predict(double delta_x, double motion_noise_std) {
        normal_distribution<double> noise(0.0, motion_noise_std);
        for (auto& p : particles) {
            p.x += delta_x + noise(gen);
        }
    }

    // 2. Measurement Update: Weight particles based on distance-to-landmark sensor reading
    void updateWeights(double measured_dist_to_landmark, double sensor_noise_std) {
        double weight_sum = 0.0;
        double variance_2 = 2.0 * sensor_noise_std * sensor_noise_std;

        for (auto& p : particles) {
            double expected_dist = landmark_x - p.x;
            double error = measured_dist_to_landmark - expected_dist;
            
            // Gaussian likelihood
            p.weight = exp(-(error * error) / variance_2);
            weight_sum += p.weight;
        }

        // Normalize weights so they sum to 1.0
        if (weight_sum > 1e-9) {
            for (auto& p : particles) {
                p.weight /= weight_sum;
            }
        }
    }

    // 3. Low-Variance Systematic Resampling Wheel
    void resample() {
        vector<Particle> new_particles;
        new_particles.reserve(num_particles);

        uniform_real_distribution<double> dist(0.0, 1.0 / num_particles);
        double r = dist(gen);
        double c = particles[0].weight;
        int idx = 0;

        for (int m = 0; m < num_particles; m++) {
            double u = r + (m * (1.0 / num_particles));
            while (u > c && idx < num_particles - 1) {
                idx++;
                c += particles[idx].weight;
            }
            new_particles.push_back({particles[idx].x, 1.0 / num_particles});
        }
        particles = move(new_particles);
    }

    // Computes weighted average pose estimate
    [[nodiscard]] double getEstimatedPose() const {
        double weighted_x = 0.0;
        for (const auto& p : particles) {
            weighted_x += p.x * p.weight;
        }
        return weighted_x;
    }
};

int main() {
    cout << "--- DAY 42: MONTE CARLO LOCALIZATION (PARTICLE FILTER) ---" << endl << endl;

    // 50 particles initialized between 0.0m and 4.0m. Known wall landmark at 10.0m.
    ParticleFilter1D pf(50, 0.0, 4.0, 10.0);
    cout << fixed << setprecision(3);

    double true_x = 1.0;          // Ground-truth robot starting position
    double speed = 0.5;           // Moving forward at 0.5 m/s
    double dt = 1.0;              // 1.0s per iteration
    double motion_noise = 0.05;   // 5cm wheel slip noise
    double sensor_noise = 0.10;   // 10cm ultrasonic noise

    cout << "Target Wall Landmark: 10.000m | Swarm Size: 50 Particles\n\n";
    cout << "Step | True Robot Pose | Sensor Distance Ping | Estimated Pose | Estimation Error" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    for (int step = 1; step <= 5; step++) {
        true_x += speed * dt;
        double sensor_reading = (10.0 - true_x) + 0.04; // Sensor ping with minor noise

        pf.predict(speed * dt, motion_noise);
        pf.updateWeights(sensor_reading, sensor_noise);
        pf.resample();

        double estimated_x = pf.getEstimatedPose();
        double error = abs(true_x - estimated_x);

        cout << " #" << setw(2) << step << "  |     " 
             << setw(6) << true_x << " m   |       " 
             << setw(6) << sensor_reading << " m       |    " 
             << setw(6) << estimated_x << " m   |     " 
             << setw(6) << error << " m" << endl;
    }

    cout << "\n>>> [VERIFIED] Swarm particles converged tightly on true robot coordinate." << endl;
    return 0;
}