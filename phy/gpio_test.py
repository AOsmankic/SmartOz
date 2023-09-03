import RPi.GPIO as GPIO

# Use P1 header pin numbering convention
GPIO.setmode(GPIO.BOARD)

# Set up the GPIO channels - one inpuyt and one output
GPIO.setup(11, GPIO.IN)
GPIO.setup(12, GPIO.OUT)


