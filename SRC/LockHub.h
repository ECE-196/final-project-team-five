#ifndef LOCKHUB_H
#define LOCKHUB_H

#include <SPI.h>
#include <Servo.h>

/**
 * @class LockHub
 * @brief Manages the locking mechanism using two servo motors.
 */
class LockHub {
    private:
        unsigned char keyCard[5]; ///< Array to store the key card data
        int straightOpenAngle; ///< Angle for straight arm in closed position
        int straightCloseAngle; ///< Angle for straight arm in open position
        int jaggedCloseAngle; ///< Angle for jagged arm in closed position
        int jaggedOpenAngle; ///< Angle for jagged arm in open position
        bool isLocked; ///< Flag to indicate if the lock is engaged
        Servo straightArm; ///< Servo motor for the straight arm
        Servo jaggedArm; ///< Servo motor for the jagged arm 

    public:
        /**
         * @brief Constructor to initialize the LockHub with two servo motors.
         * @param straightArmPin Pin number for the straight arm servo motor.
         * @param jaggedArmPin Pin number for the jagged arm servo motor.
         */
        LockHub(int straightArmPin, int jaggedArmPin);

        /**
         * @brief Set the angles for the arms in the closed position.
         * @param straight Angle for the straight arm in closed position.
         * @param jagged Angle for the jagged arm in closed position.
         */
        void setCloseAngles(int straight, int jagged);

        /**
         * @brief Set the angles for the arms in the open position.
         * @param straight Angle for the straight arm in open position.
         * @param jagged Angle for the jagged arm in open position.
         */
        void setOpenAngles(int straight, int jagged);

        /**
         * @brief Restart the lock mechanism, opens lock.
         */
        void lockRestart();

        /**
         * @brief Engage the lock using the provided key card.
         * @param card Array containing the key card data.
         */
        void lockOn(unsigned char card[5]);

        /**
         * @brief Disengage the lock using the provided key card.
         * @param card Array containing the key card data.
         */
        void lockOff(unsigned char card[5]);

        /**
         * @brief Check if the lock is currently engaged.
         * @return True if the lock is engaged, false otherwise.
         */
        bool isLocked();

        /**
         * @brief Verify if the provided key card matches the stored key card.
         * @param card Array containing the key card data.
         * @return True if the key card matches, false otherwise.
         */
        bool isKey(unsigned char card[5]);
};

#endif // LOCKHUB_H