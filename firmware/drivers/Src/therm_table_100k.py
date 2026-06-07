import math

# --- CONFIGURATION ---
ADC_RESOLUTION = 12       # 12-bit ADC (0 to 4095)
R_FIXED = 100000.0        # 100k fixed pull-up resistor
R25 = 100000.0            # 100k thermistor nominal resistance at 25°C
BETA = 3950.0             # Beta value from datasheet
T25 = 25.0 + 273.15       # 25°C converted to Kelvin
OUTPUT_FILENAME = "tempTable100k.h"

max_adc_value = (1 << ADC_RESOLUTION) - 1  # 4095
lut = []

for adc in range(max_adc_value + 1):
    # Handle mathematical boundaries at the absolute circuit limits
    if adc == 0:
        r_thermistor = R_FIXED * max_adc_value 
    elif adc == max_adc_value:
        r_thermistor = 0.001 
    else:
        # Topology A derivation: V_out = V_cc * (R_therm / (R_fixed + R_therm))
        r_thermistor = R_FIXED * (adc / (max_adc_value - adc))

    try:
        # Beta parameter equation: 1/T = 1/T25 + 1/Beta * ln(R/R25)
        one_over_t = (1.0 / T25) + (1.0 / BETA) * math.log(r_thermistor / R25)
        t_kelvin = 1.0 / one_over_t
        t_celsius = t_kelvin - 273.15
        
        # Convert to millicelsius and round to the nearest integer
        t_millicelsius = int(round(t_celsius * 1000))
    except (ValueError, ZeroDivisionError):
        t_millicelsius = lut[-1] if lut else 0

    lut.append(t_millicelsius)

# --- WRITE TO FILE ---
with open(OUTPUT_FILENAME, "w") as f:
    # Header Guards
    f.write("#ifndef TEMP_TABLE_100K_H\n")
    f.write("#define TEMP_TABLE_100K_H\n\n")
    f.write("#include <stdint.h>\n\n")
    
    f.write("// 12-bit ADC Thermistor Lookup Table (Millicelsius)\n")
    f.write("// Circuit: 3.3V -> 100k Resistor -> ADC Pin -> Thermistor -> GND\n")
    f.write(f"// Total elements: {len(lut)}\n")
    f.write("const int32_t temp_table_100k = {\n")

    # Format data in rows of 8 for neatness
    for i in range(0, len(lut), 8):
        chunk = lut[i:i+8]
        line = ", ".join(f"{val:6d}" for val in chunk)
        trailing_comma = "," if i + 8 < len(lut) else ""
        f.write(f"    {line}{trailing_comma}\n")

    f.write("};\n\n")
    f.write("#endif // TEMP_TABLE_100K_H\n")

print(f"Successfully generated {OUTPUT_FILENAME}")