#include "LockHub.h"

/**
 * @brief Constructor to initialize the LockHub with two servo motors.
 * @param straightArmPin Pin number for the straight arm servo motor.
 * @param jaggedArmPin Pin number for the jagged arm servo motor.
 */
LockHub::LockHub(int straightArmPin, int jaggedArmPin) {
    straightArm.attach(straightArmPin);
    jaggedArm.attach(jaggedArmPin);
    isLocked = false;
}

/**
 * @brief Set the angles for the arms in the closed position.
 * @param straight Angle for the straight arm in closed position.
 * @param jagged Angle for the jagged arm in closed position.
 */
void LockHub::setCloseAngles(int straight, int jagged) {
    straightCloseAngle = straight;
    jaggedCloseAngle = jagged;
    closeAngleSet = true;
}

/**
 * @brief Set the angles for the arms in the open position.
 * @param straight Angle for the straight arm in open position.
 * @param jagged Angle for the jagged arm in open position.
 */
void LockHub::setOpenAngles(int straight, int jagged) {
    straightOpenAngle = straight;
    jaggedOpenAngle = jagged;
    openAngleSet = true;
}

/**
 * @brief Restart the lock mechanism.
 */
void LockHub::lockRestart() {
    if (!openAngleSet && !closeAngleSet) {
        throw "Open and close angles not set";
    }
    lockOff();
}

/**
 * @brief Engage the lock using the provided key card.
 * @param card Array containing the key card data.
 */
void LockHub::lockOn() {
    if (!openAngleSet && !closeAngleSet) {
        throw "Open and close angles not set";
    }
    lockRestart();
    straightArm.write(straightCloseAngle);
    delay(1000);
    jaggedArm.write(straightCloseAngle);
    isLocked = true;
}

/**
 * @brief Disengage the lock using the provided key card.
 * @param card Array containing the key card data.
 */
void LockHub::lockOff() {
    if (!openAngleSet && !closeAngleSet) {
        throw "Open and close angles not set";
    }
    straightArm.write(straightOpenAngle);
    delay(1000);
    jaggedArm.write(jaggedOpenAngle);
    isLocked = false;
}

/**
 * @brief Check if the lock is currently engaged.
 * @return True if the lock is engaged, false otherwise.
 */
bool LockHub::isLocked() {
    return isLocked;
}

/**
 * @brief Verify if the provided key card matches the stored key card.
 * @param card Array containing the key card data.
 * @return True if the key card matches, false otherwise.
 */
bool LockHub::isKey(unsigned char card[5]) {
    for (int i = 0; i < 5; i++) {
        if (keyCard[i] != card[i]) {
            return false;
        }
    }
    return true;
}