/********
 * VariPass Variables
 *******/

/* VariPass account key. You can obtain this on the varipass.org website. */
#define VARIPASS_KEY "aaaaaaaa00000000"

/* VariPass variable ID. */
#define VARIPASS_ID "aaaa0000"

/* VariPass variable type. Available types are: "int", "float", "bool"
 * Defining "int" or "float" will allow for the switch to be turned on when the comparison is fulfilled. */
#define VARIPASS_TYPE "bool"

/* The compasiron operator to be used. Avilable options are: <, <=, >=, >, ==
 * This configuration is available only when "int" or "float" variable types are defined above. */
#define VARIPASS_COMPARATOR <

/* The comparison operator defined above will be used to compare to the value set here.
 * This configuration is available only when int or float variable types are defined above. */
#define VARIPASS_VALUE 20