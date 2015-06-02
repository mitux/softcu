hadoop jar /opt/cloudera/parcels/CDH-5.3.3-1.cdh5.3.3.p0.5/lib/hadoop-0.20-mapreduce/contrib/streaming/hadoop-streaming.jar \
        -file mapper.py    -mapper mapper.py \
        -file reducer.py   -reducer reducer.py \
        -input airports/input -output airports/output \
        -numReduceTasks 1 