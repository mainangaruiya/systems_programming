#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

int main() {
    union REGS regs_in, regs_out;
    int x = 0, y = 0, click = 0;
    int graphic_driver = DETECT, graphic_mode;
    float operand1 = 0, operand2 = 0, result = 0;
    int current_input = 0, decimal = 0, decimal_place = 1;
    int operation = 0;
    char display[20] = "0";
    
    initgraph(&graphic_driver, &graphic_mode, "//turboc3/bgi");
    
    // Mouse initialization
    regs_in.x.ax = 0;
    int86(0x33, &regs_in, &regs_out);
    regs_in.x.ax = 1;
    int86(0x33, &regs_in, &regs_out);
    
    // UI Setup
    setbkcolor(LIGHTGRAY);
    setcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    bar(100, 100, 390, 455);
    
    setfillstyle(SOLID_FILL, YELLOW);
    bar(110, 105, 380, 140);
    
    // Draw buttons
    char* buttons[5][6] = {
        {"7", "8", "9", "+", "-", "C"},
        {"4", "5", "6", "*", "/", "="},
        {"1", "2", "3", "%", "+/-", "."},
        {"0", "", "", "", "", ""}
    };
    
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 6; col++) {
            if(row == 3 && col > 1) continue;  // Skip empty buttons in last row
            
            int x1 = 110 + col*45;
            int y1 = 155 + row*55;
            int x2 = x1 + 40;
            int y2 = y1 + 40;
            
            setfillstyle(SOLID_FILL, LIGHTCYAN);
            bar(x1, y1, x2, y2);
            
            if(buttons[row][col][0] != '\0') {
                setcolor(BLACK);
                settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                outtextxy(x1+15, y1+15, buttons[row][col]);
            }
        }
    }
    
    // Display initial "0"
    setcolor(BLACK);
    outtextxy(115, 115, display);
    
    while(1) {
        regs_in.x.ax = 3;
        int86(0x33, &regs_in, &regs_out);
        click = regs_out.x.bx;
        x = regs_out.x.cx;
        y = regs_out.y.dx;
        
        if(click == 1) {
            // Handle button clicks
            if(y >= 155 && y <= 455) {
                int row = (y - 155) / 55;
                int col = (x - 110) / 45;
                
                if(row >= 0 && row < 4 && col >= 0 && col < 6) {
                    char* button = buttons[row][col];
                    
                    // Number input
                    if(button[0] >= '0' && button[0] <= '9') {
                        if(current_input == 0) {
                            current_input = 1;
                            strcpy(display, "");
                        }
                        
                        // Handle new input after calculation
                        if(operation == 0 && result != 0) {
                            operand1 = result;
                            result = 0;
                        }
                        
                        strcat(display, button);
                        outtextxy(115, 115, display);
                    }
                    
                    // Decimal point
                    else if(strcmp(button, ".") == 0) {
                        if(!decimal) {
                            decimal = 1;
                            strcat(display, ".");
                            outtextxy(115, 115, display);
                        }
                    }
                    
                    // Operations
                    else if(strchr("+-*/%", button[0]) != NULL) {
                        operand1 = atof(display);
                        operation = button[0];
                        current_input = 0;
                        decimal = 0;
                        decimal_place = 1;
                        strcpy(display, "0");
                    }
                    
                    // Equals
                    else if(strcmp(button, "=") == 0) {
                        operand2 = atof(display);
                        
                        switch(operation) {
                            case '+': result = operand1 + operand2; break;
                            case '-': result = operand1 - operand2; break;
                            case '*': result = operand1 * operand2; break;
                            case '/': 
                                if(operand2 != 0) result = operand1 / operand2; 
                                else strcpy(display, "Error");
                                break;
                            case '%': 
                                if(operand2 != 0) result = (int)operand1 % (int)operand2; 
                                else strcpy(display, "Error");
                                break;
                        }
                        
                        // Update display
                        if(strcmp(display, "Error") != 0) {
                            // Check if result is integer
                            if(result == (int)result) 
                                sprintf(display, "%d", (int)result);
                            else 
                                sprintf(display, "%.6f", result);
                        }
                        
                        setfillstyle(SOLID_FILL, YELLOW);
                        bar(110, 105, 380, 140);
                        outtextxy(115, 115, display);
                        
                        operation = 0;
                        current_input = 0;
                    }
                    
                    // Clear
                    else if(strcmp(button, "C") == 0) {
                        operand1 = operand2 = result = 0;
                        operation = 0;
                        current_input = 0;
                        decimal = 0;
                        decimal_place = 1;
                        strcpy(display, "0");
                        setfillstyle(SOLID_FILL, YELLOW);
                        bar(110, 105, 380, 140);
                        outtextxy(115, 115, display);
                    }
                    
                    // Sign change
                    else if(strcmp(button, "+/-") == 0) {
                        float value = atof(display);
                        value = -value;
                        
                        if(value == (int)value) 
                            sprintf(display, "%d", (int)value);
                        else 
                            sprintf(display, "%.6f", value);
                        
                        setfillstyle(SOLID_FILL, YELLOW);
                        bar(110, 105, 380, 140);
                        outtextxy(115, 115, display);
                    }
                }
            }
            delay(200); // Debounce
        }
    }
    
    closegraph();
    return 0;
}