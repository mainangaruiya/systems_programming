#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// Function prototypes
double parse_expression(const char **expr);
double parse_term(const char **expr);
double parse_factor(const char **expr);
void skip_spaces(const char **expr);
int get_operator_precedence(char op);

int main() {
    printf("Calculator Program (supports +, -, *, /, %%)\n");
    printf("Type 'exit' to quit\n\n");

    while (1) {
        printf("Enter expression: ");
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        // Remove newline character
        input[strcspn(input, "\n")] = 0;
        
        // Exit condition
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            break;
        }
        
        // Handle empty input
        if (strlen(input) == 0) {
            continue;
        }
        
        const char *expr = input;
        double result = 0.0;
        bool error = false;
        
        // Parse and evaluate the expression
        result = parse_expression(&expr);
        
        // Check for parsing errors
        skip_spaces(&expr);
        if (*expr != '\0') {
            printf("Error: Unexpected character '%c' at position %ld\n", *expr, expr - input + 1);
            error = true;
        }
        
        // Display result if no errors
        if (!error) {
            // Format output appropriately
            if (result == (long)result) {
                printf("Result: %ld\n", (long)result);
            } else {
                printf("Result: %.6g\n", result);
            }
        }
    }
    
    printf("Calculator exited.\n");
    return 0;
}

// Skip whitespace characters
void skip_spaces(const char **expr) {
    while (isspace(**expr)) {
        (*expr)++;
    }
}

// Get operator precedence
int get_operator_precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': case '%': return 2;
        default: return 0;
    }
}

// Parse an expression
double parse_expression(const char **expr) {
    double left = parse_term(expr);
    
    while (true) {
        skip_spaces(expr);
        char op = **expr;
        
        // Check if we have a valid operator
        if (op != '+' && op != '-') {
            break;
        }
        
        // Move to next character
        (*expr)++;
        
        double right = parse_term(expr);
        
        // Perform the operation
        if (op == '+') {
            left += right;
        } else {
            left -= right;
        }
    }
    
    return left;
}

// Parse a term
double parse_term(const char **expr) {
    double left = parse_factor(expr);
    
    while (true) {
        skip_spaces(expr);
        char op = **expr;
        
        // Check if we have a valid operator
        if (op != '*' && op != '/' && op != '%') {
            break;
        }
        
        // Move to next character
        (*expr)++;
        
        double right = parse_factor(expr);
        
        // Perform the operation
        if (op == '*') {
            left *= right;
        } else if (op == '/') {
            if (right == 0.0) {
                printf("Error: Division by zero\n");
                return NAN; // Not-a-Number to indicate error
            }
            left /= right;
        } else if (op == '%') {
            if (right == 0.0) {
                printf("Error: Modulus by zero\n");
                return NAN;
            }
            left = fmod(left, right);
        }
    }
    
    return left;
}

// Parse a factor (number or subexpression)
double parse_factor(const char **expr) {
    skip_spaces(expr);
    
    // Handle negative numbers
    int sign = 1;
    if (**expr == '-') {
        sign = -1;
        (*expr)++;
        skip_spaces(expr);
    }
    
    // Handle parentheses
    if (**expr == '(') {
        (*expr)++;
        double result = parse_expression(expr);
        skip_spaces(expr);
        if (**expr != ')') {
            printf("Error: Missing closing parenthesis\n");
            return NAN;
        }
        (*expr)++;
        return sign * result;
    }
    
    // Parse a number
    char *endptr;
    double value = strtod(*expr, &endptr);
    
    if (endptr == *expr) {
        printf("Error: Expected number\n");
        return NAN;
    }
    
    *expr = endptr;
    return sign * value;
}