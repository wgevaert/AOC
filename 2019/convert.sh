for file in $(ls | grep 'pgm$'); do convert $file -filter point -resize 1000% $file.png;done
