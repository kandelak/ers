### RUN ERS

    $ ./bin/ers_cli --help
    Allowed options:
      --help                   produce help message
      --input arg              the folder to process
      --lambda arg (=0.5)      lambda
      --sigma arg (=5)         sigma
      --four-connected         use 4-connected
      --superpixels arg (=400) number of superpixels
      --time arg               time the algorithm and save results to the given 
                               directory
      --process                show additional information while processing
      --csv                    save segmentation as CSV file
      --contour                save contour image of segmentation
      --mean                   save mean colored image of segmentation
      --output arg (=output)   specify the output directory (default is ./output)