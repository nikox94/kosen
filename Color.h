#ifndef COLOR_H
#define COLOR_H

class Color {
	double red, green, blue, special;
	
	public:
	
	Color ();
	
	Color (double, double, double, double);
	
	// method functions
	double getRed() { return red; }
	double getGreen() { return green; }
	double getBlue() { return blue; }
	double getSpecial() { return special; }
	
	double setRed(double redValue) { red = redValue; }
	double setGreen(double greenValue) { green = greenValue; }
	double setBlue(double blueValue) { blue = blueValue; }
	double setSpecial(double specialValue) { special = specialValue; }
	
	double brightness() {
		return (red + green + blue)/3;
	}
	
	Color scale(double scalar) {
		return Color (red*scalar, green*scalar, blue*scalar, special);
	}
	
	Color add (Color color) {
		return Color (red + color.red, green + color.green, blue + color.blue, special);
	}
	
	Color multiply (Color color) {
		return Color (red*color.red, green*color.green, blue*color.blue, special);
	}
	
	Color colorAverage(Color color) {
		return Color ((red + color.red)/2, (green + color.green)/2, (blue + color.blue)/2, special);
	}
	
	Color clip() {
		double alllight = red + green + blue;
		double excesslight = alllight - 3;
		if (excesslight > 0) {
			red = red + excesslight*(red/alllight);
			green = green + excesslight*(green/alllight);
			blue = blue + excesslight*(blue/alllight);
		}
		if (red > 1) {red = 1;}
		if (green > 1) {green = 1;}
		if (blue > 1) {blue = 1;}
		if (red < 0) {red = 0;}
		if (green < 0) {green = 0;}
		if (blue < 0) {blue = 0;}
		
		return Color (red, green, blue, special);
	}
};

Color::Color () {
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color (double r, double g, double b, double s) {
	red = r;
	green = g;
	blue = b;
	special = s;
}

#endif