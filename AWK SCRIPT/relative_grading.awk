awk -F',' '{
    ISEsum += ($3+$6+$9+$12+$15);
    ISEmean=ISEsum/100;
    ISEdiff+=((($3+$6+$9+$12+$15)-ISEmean)*(($3+$6+$9+$12+$15)-ISEmean));
    ISEstd=sqrt(ISEdiff/(99));
    MSEsum+=($4+$7+$10+$13+$16);
    MSEmean=MSEsum/100;
    MSEdiff+=((($4+$7+$10+$13+$16)-MSEmean)*(($4+$7+$10+$13+$16)-MSEmean));
    MSEstd=sqrt(MSEdiff/(99));
    ESEsum+=($5+$8+$11+$14+$17);
    ESEmean=ESEsum/100;
    ESEdiff+=((($5+$8+$11+$14+$17)-ESEmean)*(($5+$8+$11+$14+$17)-ESEmean));
    ESEstd=sqrt(ESEdiff/(99));
    TOTALsum += (($3+$6+$9+$12+$15)/5 + ($4+$7+$10+$13+$16)/5 + ($5+$8+$11+$14+$17)/5);
    TOTALmean = TOTALsum/100;
    TOTALdiff += (((($3+$6+$9+$12+$15)/5 + ($4+$7+$10+$13+$16)/5 + ($5+$8+$11+$14+$17)/5) - TOTALmean)*((($3+$6+$9+$12+$15)/5 + ($4+$7+$10+$13+$16)/5 + ($5+$8+$11+$14+$17)/5) - TOTALmean));
    TOTALstd = sqrt(TOTALdiff/99);
} 
END{
    print "ISE mean of all students in all 5 subjects is " ISEmean/5 " out of 20";
    print "ISE standard deviation will be " ISEstd;
    print "Grade C in ISE to the students having avg ISE marks below  " ISEmean/5;
    print "Grade B in ISE to the students having avg ISE marks below " (ISEmean+(ISEstd/4))/5 " and above "ISEmean/5;
    print "Grade A in ISE to the students having avg ISE marks above " (ISEmean+(ISEstd/4))/5;
    print "Grade D in ISE to the students having avg ISE marks below " (ISEmean-(3*ISEstd/4))/5;
    print "MSE mean of all students in all 5 subjects is " MSEmean/5 " out of 30";
    print "MSE standard deviation will be " MSEstd;
    print "Grade C in MSE to the students having avg MSE marks below  " MSEmean/5;
    print "Grade B in MSE to the students having avg MSE marks below " (MSEmean+(MSEstd/4))/5 " and above "MSEmean/5;
    print "Grade A in MSE to the students having avg MSE marks above " (MSEmean+(MSEstd/4))/5;
    print "Grade D in MSE to the students having avg MSE marks below " (MSEmean-(3*MSEstd/4))/5;
    print "ESE mean of all students in all 5 subjects is " ESEmean/5 " out of 50";
    print "ESE standard deviation will be " ESEstd;
    print "Grade C in ESE to the students having avg ESE marks below  " ESEmean/5;
    print "Grade B in ESE to the students having avg ESE marks below " (ESEmean+(ESEstd/4))/5 " and above "ESEmean/5;
    print "Grade A in ESE to the students having avg ESE marks above " (ESEmean+(ESEstd/4))/5;
    print "Grade D in ESE to the students having avg ESE marks below " (ESEmean-(3*ESEstd/4))/5;
    print "Percentage mean of all students in all 5 subjects is " TOTALmean " out of 50";
    print "Percentage standard deviation will be " TOTALstd;
    print "Grade C in final exam to the students having percentage below  " TOTALmean;
    print "Grade B in final exam to the students having percentage below " (TOTALmean+(TOTALstd/4)) " and above "TOTALmean;
    print "Grade A in final exam to the students having percentage above " (TOTALmean+(TOTALstd/4));
    print "Grade D in final exam to the students having percentage below " (TOTALmean-(3*TOTALstd/4));
}' grades.txt

