less 11.cpp && g++ -Wall -Wextra 11.cpp && ./a.out 0 94 76 190 135 < /tmp/input.txt > meuk.txt && rm $(ls | grep -o '[0-9][0-9][0-9][0-9]\.pgm$' | grep -v '[0-9][0-9][24680]0\.pgm$' | grep -v $(ls | grep -o '[0-9][0-9][0-9][0-9]\.pgm$' | tail -n1)) && ./convert.sh && convert -delay 2 $(ls | grep -o '[0-9][0-9][0-9][0-9]\.pgm\.png$') $(ls | grep -o '[0-9][0-9][0-9][0-9]\.pgm\.png$' | tail -n1) $(ls | grep -o '[0-9][0-9][0-9][0-9]\.pgm\.png$' | tail -n1) $(ls | grep -o '[0-9][0-9][0-9][0-9]\.pgm\.png$' | tail -n1) $(ls | grep -0 '[0-9][0-9][0-9][0-9]\.pgm\.png$' | tail -n1) $(ls | grep -0 '[0-9][0-9][0-9][0-9]\.pgm\.png$' | tail -n1) -loop 0 fractal.gif; rm $(ls | grep -o '[0-9][0-9][0-9][0-9]\.pgm$')
