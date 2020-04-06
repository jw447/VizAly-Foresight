import sys
import numpy as np
import matplotlib.pyplot as plt
y=[1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 2.000000, 1.000000, 1.000000, 1.000000, 1.000000, 2.000000, 3.000000, 0.000000, 4.000000, 2.000000, 1.000000, 2.000000, 0.000000, 2.000000, 0.000000, 1.000000, 2.000000, 2.000000, 6.000000, 1.000000, 1.000000, 0.000000, 0.000000, 2.000000, 1.000000, 4.000000, 6.000000, 2.000000, 3.000000, 2.000000, 0.000000, 4.000000, 5.000000, 4.000000, 1.000000, 2.000000, 4.000000, 4.000000, 5.000000, 5.000000, 4.000000, 8.000000, 8.000000, 3.000000, 8.000000, 14.000000, 6.000000, 8.000000, 7.000000, 6.000000, 14.000000, 6.000000, 11.000000, 10.000000, 4.000000, 8.000000, 13.000000, 12.000000, 12.000000, 6.000000, 8.000000, 12.000000, 9.000000, 16.000000, 11.000000, 20.000000, 11.000000, 14.000000, 12.000000, 12.000000, 17.000000, 24.000000, 16.000000, 24.000000, 24.000000, 23.000000, 29.000000, 20.000000, 19.000000, 25.000000, 27.000000, 26.000000, 30.000000, 24.000000, 21.000000, 32.000000, 30.000000, 27.000000, 35.000000, 31.000000, 25.000000, 45.000000, 47.000000, 42.000000, 38.000000, 43.000000, 42.000000, 49.000000, 42.000000, 50.000000, 44.000000, 47.000000, 56.000000, 59.000000, 63.000000, 58.000000, 73.000000, 74.000000, 59.000000, 54.000000, 87.000000, 68.000000, 69.000000, 72.000000, 86.000000, 91.000000, 82.000000, 96.000000, 91.000000, 88.000000, 100.000000, 107.000000, 104.000000, 110.000000, 94.000000, 123.000000, 137.000000, 124.000000, 135.000000, 134.000000, 133.000000, 158.000000, 149.000000, 165.000000, 154.000000, 174.000000, 167.000000, 163.000000, 181.000000, 181.000000, 193.000000, 198.000000, 197.000000, 220.000000, 195.000000, 233.000000, 241.000000, 230.000000, 263.000000, 253.000000, 249.000000, 249.000000, 262.000000, 294.000000, 295.000000, 283.000000, 325.000000, 332.000000, 331.000000, 331.000000, 347.000000, 355.000000, 366.000000, 379.000000, 399.000000, 439.000000, 409.000000, 443.000000, 438.000000, 480.000000, 481.000000, 516.000000, 566.000000, 510.000000, 551.000000, 592.000000, 604.000000, 654.000000, 650.000000, 681.000000, 690.000000, 728.000000, 713.000000, 778.000000, 792.000000, 781.000000, 924.000000, 872.000000, 872.000000, 968.000000, 941.000000, 993.000000, 1077.000000, 1142.000000, 1120.000000, 1192.000000, 1242.000000, 1315.000000, 1364.000000, 1383.000000, 1451.000000, 1471.000000, 1525.000000, 1559.000000, 1641.000000, 1614.000000, 1713.000000, 1815.000000, 1907.000000, 1858.000000, 1958.000000, 2028.000000, 2082.000000, 2109.000000, 2182.000000, 2261.000000, 2336.000000, 2420.000000, 2477.000000, 2594.000000, 2696.000000, 2859.000000, 2775.000000, 3078.000000, 2986.000000, 3181.000000, 3252.000000, 3409.000000, 3588.000000, 3580.000000, 3786.000000, 3904.000000, 4095.000000, 4183.000000, 4174.000000, 4544.000000, 4658.000000, 4824.000000, 4942.000000, 5164.000000, 5398.000000, 5414.000000, 5667.000000, 5939.000000, 5937.000000, 6399.000000, 6460.000000, 6666.000000, 7185.000000, 7270.000000, 7392.000000, 7704.000000, 8065.000000, 8207.000000, 8487.000000, 8675.000000, 9073.000000, 9286.000000, 9581.000000, 9932.000000, 10265.000000, 10544.000000, 10879.000000, 11617.000000, 11670.000000, 12477.000000, 12678.000000, 13438.000000, 13820.000000, 14349.000000, 14742.000000, 15422.000000, 15699.000000, 16214.000000, 16836.000000, 17425.000000, 18131.000000, 18570.000000, 19386.000000, 19916.000000, 20475.000000, 21186.000000, 21313.000000, 22356.000000, 22606.000000, 23709.000000, 24128.000000, 25064.000000, 25872.000000, 26314.000000, 27253.000000, 28210.000000, 28918.000000, 29938.000000, 31131.000000, 31783.000000, 32907.000000, 34069.000000, 34878.000000, 36315.000000, 37280.000000, 38745.000000, 39954.000000, 41481.000000, 43060.000000, 45030.000000, 46944.000000, 48910.000000, 50779.000000, 52609.000000, 55030.000000, 57427.000000, 59798.000000, 62500.000000, 65105.000000, 67597.000000, 70745.000000, 73951.000000, 76735.000000, 80411.000000, 84297.000000, 87706.000000, 92598.000000, 97982.000000, 103095.000000, 110701.000000, 116368.000000, 123521.000000, 130088.000000, 138220.000000, 146387.000000, 155815.000000, 165857.000000, 174862.000000, 184915.000000, 197735.000000, 211295.000000, 224215.000000, 236963.000000, 250703.000000, 263080.000000, 278466.000000, 293315.000000, 305529.000000, 320316.000000, 336737.000000, 352965.000000, 368222.000000, 381138.000000, 397784.000000, 414003.000000, 428930.000000, 446685.000000, 467114.000000, 485190.000000, 503885.000000, 523804.000000, 542826.000000, 565776.000000, 590217.000000, 613919.000000, 637262.000000, 661799.000000, 684056.000000, 707521.000000, 739877.000000, 776250.000000, 807856.000000, 842548.000000, 875586.000000, 915972.000000, 952964.000000, 996038.000000, 1033263.000000, 1063517.000000, 1097671.000000, 1134741.000000, 1168625.000000, 1202029.000000, 1235227.000000, 1279208.000000, 1328522.000000, 1368946.000000, 1414108.000000, 1450214.000000, 1486731.000000, 1523801.000000, 1566068.000000, 1607622.000000, 1643731.000000, 1682539.000000, 1726096.000000, 1772894.000000, 1830650.000000, 1887250.000000, 1943572.000000, 2011350.000000, 2077987.000000, 2144296.000000, 2216045.000000, 2297046.000000, 2383459.000000, 2464973.000000, 2537998.000000, 2616069.000000, 2695057.000000, 2773698.000000, 2862371.000000, 2949164.000000, 3028170.000000, 3117591.000000, 3213786.000000, 3300793.000000, 3389603.000000, 3473765.000000, 3561330.000000, 3653048.000000, 3746879.000000, 3837406.000000, 3926375.000000, 4022438.000000, 4090665.000000, 4154631.000000, 4228560.000000, 4304496.000000, 4373091.000000, 4440430.000000, 4492909.000000, 4562254.000000, 4630406.000000, 4677128.000000, 4744307.000000, 4801718.000000, 4869152.000000, 4955689.000000, 5041064.000000, 5087315.000000, 5137520.000000, 5176058.000000, 5232380.000000, 5287841.000000, 5329963.000000, 5375141.000000, 5414784.000000, 5442846.000000, 5470083.000000, 5482643.000000, 5498408.000000, 5529980.000000, 5573760.000000, 5631398.000000, 5694037.000000, 5738968.000000, 5772706.000000, 5811235.000000, 5856069.000000, 5882856.000000, 5908988.000000, 5952535.000000, 5975563.000000, 5993143.000000, 6024881.000000, 6060866.000000, 6089494.000000, 6106957.000000, 6134447.000000, 6162998.000000, 6216669.000000, 6274565.000000, 6320663.000000, 6372951.000000, 6396148.000000, 6439248.000000, 6499252.000000, 6522809.000000, 6530407.000000, 6514438.000000, 6486609.000000, 6465048.000000, 6441400.000000, 6427355.000000, 6407512.000000, 6348445.000000, 6319016.000000, 6279673.000000, 6236923.000000, 6188086.000000, 6133400.000000, 6086402.000000, 6052379.000000, 6016762.000000, 5979437.000000, 5931667.000000, 5903071.000000, 5875386.000000, 5867682.000000, 5859768.000000, 5863811.000000, 5865701.000000, 5844069.000000, 5838116.000000, 5821849.000000, 5802820.000000, 5773199.000000, 5737183.000000, 5719190.000000, 5698400.000000, 5666281.000000, 5600632.000000, 5534500.000000, 5474515.000000, 5424429.000000, 5373779.000000, 5351757.000000, 5324544.000000, 5304336.000000, 5274548.000000, 5238367.000000, 5197422.000000, 5178748.000000, 5146444.000000, 5115794.000000, 5066794.000000, 5019966.000000, 4966007.000000, 4905811.000000, 4849945.000000, 4779891.000000, 4708535.000000, 4643417.000000, 4593061.000000, 4551149.000000, 4519982.000000, 4501872.000000, 4483662.000000, 4465938.000000, 4430856.000000, 4395489.000000, 4366984.000000, 4336711.000000, 4295352.000000, 4268840.000000, 4224422.000000, 4184567.000000, 4151651.000000, 4123796.000000, 4085878.000000, 4055147.000000, 4015724.000000, 3980063.000000, 3941062.000000, 3888471.000000, 3835432.000000, 3797867.000000, 3770568.000000, 3731189.000000, 3695964.000000, 3676987.000000, 3664133.000000, 3653839.000000, 3649173.000000, 3637096.000000, 3625768.000000, 3602330.000000, 3573132.000000, 3555345.000000, 3533639.000000, 3503170.000000, 3472176.000000, 3435252.000000, 3406630.000000, 3379214.000000, 3340969.000000, 3310608.000000, 3277166.000000, 3237180.000000, 3202470.000000, 3161739.000000, 3127061.000000, 3094661.000000, 3066430.000000, 3050273.000000, 3019861.000000, 2988128.000000, 2959752.000000, 2925237.000000, 2893633.000000, 2873761.000000, 2850594.000000, 2823499.000000, 2793970.000000, 2773140.000000, 2750113.000000, 2721806.000000, 2693955.000000, 2656011.000000, 2625781.000000, 2597208.000000, 2571038.000000, 2550965.000000, 2522386.000000, 2487611.000000, 2462170.000000, 2438404.000000, 2409129.000000, 2362489.000000, 2315191.000000, 2264709.000000, 2218898.000000, 2169203.000000, 2121123.000000, 2058032.000000, 1996207.000000, 1944959.000000, 1894268.000000, 1850028.000000, 1805301.000000, 1759203.000000, 1716269.000000, 1660290.000000, 1608225.000000, 1560033.000000, 1515313.000000, 1470921.000000, 1422275.000000, 1372999.000000, 1329107.000000, 1287886.000000, 1249650.000000, 1213172.000000, 1167652.000000, 1120490.000000, 1075283.000000, 1038607.000000, 1004245.000000, 966500.000000, 928146.000000, 892917.000000, 863466.000000, 827058.000000, 787829.000000, 754879.000000, 723129.000000, 691513.000000, 661161.000000, 631502.000000, 607534.000000, 583888.000000, 557430.000000, 529243.000000, 502498.000000, 480930.000000, 455538.000000, 436750.000000, 419683.000000, 397284.000000, 379673.000000, 361856.000000, 345643.000000, 328746.000000, 315441.000000, 302846.000000, 291531.000000, 281846.000000, 274696.000000, 265503.000000, 256272.000000, 248587.000000, 241583.000000, 236466.000000, 231026.000000, 225053.000000, 215477.000000, 205297.000000, 196119.000000, 188389.000000, 184057.000000, 179414.000000, 174893.000000, 168094.000000, 162331.000000, 156381.000000, 150865.000000, 142282.000000, 137082.000000, 130369.000000, 124951.000000, 119533.000000, 115451.000000, 111558.000000, 107706.000000, 103973.000000, 100169.000000, 95279.000000, 91845.000000, 87456.000000, 82990.000000, 79888.000000, 76787.000000, 74595.000000, 71578.000000, 68756.000000, 67192.000000, 65249.000000, 62378.000000, 60514.000000, 59386.000000, 58752.000000, 55928.000000, 53159.000000, 50714.000000, 48388.000000, 46093.000000, 43539.000000, 41583.000000, 39795.000000, 38118.000000, 35975.000000, 34344.000000, 32815.000000, 30625.000000, 28880.000000, 27499.000000, 25876.000000, 25064.000000, 24028.000000, 23197.000000, 21983.000000, 20398.000000, 19201.000000, 18417.000000, 17050.000000, 16052.000000, 14821.000000, 13574.000000, 12656.000000, 11610.000000, 10913.000000, 10098.000000, 9458.000000, 8702.000000, 8256.000000, 7660.000000, 7297.000000, 6685.000000, 6529.000000, 6084.000000, 5937.000000, 5547.000000, 5356.000000, 4847.000000, 4662.000000, 4405.000000, 4021.000000, 3893.000000, 3524.000000, 3422.000000, 3104.000000, 3037.000000, 2814.000000, 2750.000000, 2523.000000, 2374.000000, 2282.000000, 2195.000000, 2130.000000, 1914.000000, 1818.000000, 1828.000000, 1698.000000, 1592.000000, 1511.000000, 1511.000000, 1441.000000, 1334.000000, 1245.000000, 1208.000000, 1244.000000, 1175.000000, 1051.000000, 1026.000000, 1061.000000, 984.000000, 891.000000, 887.000000, 800.000000, 883.000000, 789.000000, 783.000000, 759.000000, 731.000000, 679.000000, 653.000000, 599.000000, 573.000000, 535.000000, 556.000000, 494.000000, 475.000000, 436.000000, 412.000000, 421.000000, 404.000000, 377.000000, 356.000000, 287.000000, 281.000000, 263.000000, 272.000000, 218.000000, 244.000000, 211.000000, 201.000000, 164.000000, 217.000000, 162.000000, 165.000000, 177.000000, 154.000000, 142.000000, 139.000000, 121.000000, 126.000000, 142.000000, 114.000000, 107.000000, 100.000000, 99.000000, 113.000000, 90.000000, 89.000000, 90.000000, 73.000000, 64.000000, 76.000000, 66.000000, 65.000000, 77.000000, 60.000000, 58.000000, 57.000000, 58.000000, 49.000000, 46.000000, 47.000000, 49.000000, 43.000000, 39.000000, 36.000000, 39.000000, 34.000000, 36.000000, 38.000000, 35.000000, 39.000000, 26.000000, 22.000000, 25.000000, 22.000000, 21.000000, 25.000000, 22.000000, 23.000000, 22.000000, 26.000000, 23.000000, 15.000000, 18.000000, 17.000000, 13.000000, 18.000000, 17.000000, 27.000000, 12.000000, 10.000000, 12.000000, 12.000000, 3.000000, 6.000000, 10.000000, 9.000000, 10.000000, 7.000000, 9.000000, 8.000000, 7.000000, 6.000000, 6.000000, 5.000000, 7.000000, 7.000000, 3.000000, 11.000000, 4.000000, 6.000000, 7.000000, 4.000000, 3.000000, 2.000000, 2.000000, 3.000000, 4.000000, 2.000000, 2.000000, 4.000000, 2.000000, 3.000000, 3.000000, 5.000000, 0.000000, 1.000000, 3.000000, 1.000000, 2.000000, 1.000000, 2.000000, 3.000000, 1.000000, 0.000000, 2.000000, 0.000000, 2.000000, 1.000000, 3.000000, 1.000000, 3.000000, 0.000000, 3.000000, 0.000000, 0.000000, 1.000000, 2.000000, 2.000000, 1.000000, 0.000000, 2.000000, 2.000000, 1.000000, 1.000000, 0.000000, 1.000000, 1.000000, 3.000000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000]
minVal=-50607724.000000
maxVal=45937300.000000
plotName=sys.argv[0]
plotName = plotName.replace('.py','.png')
numVals = len(y)
x = np.linspace(minVal, maxVal, numVals+1)[1:]
plt.semilogy(x,y, linewidth=0.5)
plt.title(plotName)
plt.ylabel("Frequency")
plt.xticks(rotation=90)
plt.tight_layout()
plt.savefig(plotName, dpi=300)