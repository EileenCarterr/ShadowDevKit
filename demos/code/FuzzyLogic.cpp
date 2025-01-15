#include <iostream>
#include <cstdlib>
#include "Variables.h"

int main() {
    try {
        // Define fuzzy shapes for Health variable
        TrapezoidShape healthLow(0.0f, 0.0f, 30.0f, 50.0f);
        TriangleShape  healthMedium(30.0f, 50.0f, 70.0f);
        TrapezoidShape healthHigh(50.0f, 70.0f, 100.0f, 100.0f);

        // Define fuzzy shapes for Enemy Density variable
        TrapezoidShape enemyLow(0.0f, 0.0f, 2.0f, 4.0f);
        TriangleShape  enemyMedium(2.0f, 5.0f, 8.0f);
        TrapezoidShape enemyHigh(6.0f, 8.0f, 10.0f, 10.0f);

        // Create Fuzzy Variables
        Variable health(healthLow, healthMedium, healthHigh);
        Variable enemies(enemyLow, enemyMedium, enemyHigh);

        // Inputs: Current health and enemy count
        float currentHealth  = 45.0f;   // Current health
        float currentEnemies = 3.0f;   // Current number of enemies nearby

        // Evaluate fuzzy variables
        health.Evaluate(currentHealth);
        enemies.Evaluate(currentEnemies);

        // Print debug information
        std::cout << "Health Variable:" << std::endl;
        health.debugPrint();
        std::cout << "Enemy Density Variable:" << std::endl;
        enemies.debugPrint();

        // Decision-making logic based on fuzzy values
        if (health.IsHigh() && enemies.IsLow()) {
            std::cout << "Decision: Fight!" << std::endl;
        } else if (health.IsLow() || enemies.IsHigh()) {
            std::cout << "Decision: Flee!" << std::endl;
        } else {
            std::cout << "Decision: Be cautious, assess further!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
