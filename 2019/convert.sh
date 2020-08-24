for file in $(ls | grep -o '[0-9][0-9][0-9][0-9]\.pgm$'); do convert $file -filter point -resize 1000% $file.png;done
