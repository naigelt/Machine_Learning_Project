extends Spatial


# List of server's nodeIds vs Godot's cities/roads/searoutes
# added in the same order as in the server code, so the order doesn't make sense here
var nodeIdArray : Array = [
	[300, $Cities/Tangier],
	[301, $Cities/Cairo],
	[302, $Cities/Morocco],
	[303, $Cities/Tunis],
	[304, $Cities/Tripoli],
	[305, $"Cities/Canary Islands"],
	[306, $Cities/Sahara],
	[307, $Cities/Egypt],
	[308, $"Cities/Cape Verde"],
	[309, $Cities/Timbuktu],
	[310, $"Cities/Ajn-Galaka"],
	[311, $Cities/Suakin],
	[312, $"Cities/Sierra Leone"],
	[313, $"Cities/Gold Coast"],
	[314, $"Cities/Slave Coast"],
	[315, $"Cities/Dar-Fur"],
	[316, $"Cities/Bahr el Ghasal"],
	[317, $"Cities/Addis Abeba"],
	[318, $"Cities/Cape Guardafui"],
	[319, $Cities/Kandjama],
	[320, $"Cities/Lake Victoria"],
	[321, $"Cities/St Helena"],
	[322, $Cities/Congo],
	[323, $Cities/Ocomba],
	[324, $Cities/Daressalam],
	[325, $"Cities/Whalefish Bay"],
	[326, $"Cities/Victoria Falls"],
	[327, $Cities/Mozambique],
	[328, $Cities/Tamatave],
	[329, $"Cities/Dragon Mountain"],
	[330, $"Cities/Cape St Marie"],
	[331, $"Cities/Cape Town"],
	[1, $Roads/RoadPoint5],
	[2, $Roads/RoadPoint6],
	[3, $Roads/RoadPoint7],
	[4, $Roads/RoadPoint8],
	[5, $Roads/RoadPoint4],
	[6, $Roads/RoadPoint3],
	[7, $Roads/RoadPoint2],
	[8, $Roads/RoadPoint],
	[200, $SeaPoints/SeaPoint],
	[201, $SeaPoints/SeaPoint2],
	[275, $SeaPoints/SeaPoint77],
	[276, $SeaPoints/SeaPoint76],
	[16, $Roads/RoadPoint109],
	[17, $Roads/RoadPoint108],
	[18, $Roads/RoadPoint107],
	[210, $SeaPoints/SeaPoint7],
	[211, $SeaPoints/SeaPoint8],
	[212, $SeaPoints/SeaPoint9],
	[9, $Roads/RoadPoint53],
	[10, $Roads/RoadPoint52],
	[202, $SeaPoints/SeaPoint3],
	[203, $SeaPoints/SeaPoint4],
	[204, $SeaPoints/SeaPoint5],
	[205, $SeaPoints/SeaPoint6],
	[11, $Roads/RoadPoint51],
	[12, $Roads/RoadPoint50],
	[13, $Roads/RoadPoint49],
	[14, $Roads/RoadPoint48],
	[15, $Roads/RoadPoint47],
	[206, $SeaPoints/SeaPoint75],
	[207, $SeaPoints/SeaPoint74],
	[208, $SeaPoints/SeaPoint73],
	[209, $SeaPoints/SeaPoint72],
	[19, $Roads/RoadPoint9],
	[20, $Roads/RoadPoint10],
	[21, $Roads/RoadPoint11],
	[22, $Roads/RoadPoint12],
	[23, $Roads/RoadPoint13],
	[24, $Roads/RoadPoint14],
	[25, $Roads/RoadPoint15],
	[26, $Roads/RoadPoint45],
	[27, $Roads/RoadPoint46],
	[28, $Roads/RoadPoint44],
	[29, $Roads/RoadPoint43],
	[30, $Roads/RoadPoint42],
	[31, $Roads/RoadPoint41],
	[32, $Roads/RoadPoint40],
	[38, $Roads/RoadPoint16],
	[39, $Roads/RoadPoint17],
	[40, $Roads/RoadPoint18],
	[33, $Roads/RoadPoint39],
	[34, $Roads/RoadPoint38],
	[35, $Roads/RoadPoint37],
	[36, $Roads/RoadPoint36],
	[37, $Roads/RoadPoint35],
	[233, $SeaPoints/SeaPoint10],
	[234, $SeaPoints/SeaPoint11],
	[235, $SeaPoints/SeaPoint12],
	[236, $SeaPoints/SeaPoint13],
	[63, $Roads/RoadPoint99],
	[64, $Roads/RoadPoint98],
	[41, $Roads/RoadPoint22],
	[42, $Roads/RoadPoint21],
	[43, $Roads/RoadPoint19],
	[44, $Roads/RoadPoint23],
	[45, $Roads/RoadPoint20],
	[213, $SeaPoints/SeaPoint71],
	[214, $SeaPoints/SeaPoint56],
	[215, $SeaPoints/SeaPoint55],
	[216, $SeaPoints/SeaPoint54],
	[217, $SeaPoints/SeaPoint53],
	[218, $SeaPoints/SeaPoint52],
	[219, $SeaPoints/SeaPoint51],
	[220, $SeaPoints/SeaPoint50],
	[221, $SeaPoints/SeaPoint49],
	[222, $SeaPoints/SeaPoint48],
	[223, $SeaPoints/SeaPoint57],
	[224, $SeaPoints/SeaPoint58],
	[225, $SeaPoints/SeaPoint59],
	[226, $SeaPoints/SeaPoint60],
	[46, $Roads/RoadPoint27],
	[47, $Roads/RoadPoint26],
	[48, $Roads/RoadPoint25],
	[49, $Roads/RoadPoint24],
	[50, $Roads/RoadPoint28],
	[51, $Roads/RoadPoint29],
	[52, $Roads/RoadPoint30],
	[53, $Roads/RoadPoint110],
	[54, $Roads/RoadPoint56],
	[55, $Roads/RoadPoint55],
	[56, $Roads/RoadPoint54],
	[57, $Roads/RoadPoint31],
	[58, $Roads/RoadPoint32],
	[59, $Roads/RoadPoint33],
	[227, $SeaPoints/SeaPoint61],
	[228, $SeaPoints/SeaPoint62],
	[229, $SeaPoints/SeaPoint63],
	[230, $SeaPoints/SeaPoint64],
	[231, $SeaPoints/SeaPoint65],
	[232, $SeaPoints/SeaPoint66],
	[60, $Roads/RoadPoint86],
	[61, $Roads/RoadPoint85],
	[62, $Roads/RoadPoint84],
	[65, $Roads/RoadPoint101],
	[66, $Roads/RoadPoint100],
	[71, $Roads/RoadPoint102],
	[72, $Roads/RoadPoint103],
	[73, $Roads/RoadPoint104],
	[74, $Roads/RoadPoint105],
	[75, $Roads/RoadPoint106],
	[237, $SeaPoints/SeaPoint21],
	[238, $SeaPoints/SeaPoint22],
	[239, $SeaPoints/SeaPoint23],
	[240, $SeaPoints/SeaPoint24],
	[241, $SeaPoints/SeaPoint25],
	[242, $SeaPoints/SeaPoint26],
	[243, $SeaPoints/SeaPoint27],
	[244, $SeaPoints/SeaPoint14],
	[245, $SeaPoints/SeaPoint15],
	[246, $SeaPoints/SeaPoint16],
	[247, $SeaPoints/SeaPoint17],
	[248, $SeaPoints/SeaPoint18],
	[249, $SeaPoints/SeaPoint19],
	[250, $SeaPoints/SeaPoint20],
	[67, $Roads/RoadPoint97],
	[68, $Roads/RoadPoint96],
	[69, $Roads/RoadPoint34],
	[70, $Roads/RoadPoint65],
	[76, $Roads/RoadPoint66],
	[77, $Roads/RoadPoint67],
	[78, $Roads/RoadPoint68],
	[79, $Roads/RoadPoint69],
	[80, $Roads/RoadPoint70],
	[81, $Roads/RoadPoint64],
	[82, $Roads/RoadPoint63],
	[83, $Roads/RoadPoint62],
	[84, $Roads/RoadPoint57],
	[85, $Roads/RoadPoint58],
	[86, $Roads/RoadPoint59],
	[87, $Roads/RoadPoint60],
	[88, $Roads/RoadPoint61],
	[89, $Roads/RoadPoint83],
	[90, $Roads/RoadPoint81],
	[91, $Roads/RoadPoint80],
	[92, $Roads/RoadPoint79],
	[93, $Roads/RoadPoint78],
	[94, $Roads/RoadPoint77],
	[95, $Roads/RoadPoint76],
	[96, $Roads/RoadPoint72],
	[97, $Roads/RoadPoint71],
	[98, $Roads/RoadPoint73],
	[99, $Roads/RoadPoint75],
	[100, $Roads/RoadPoint95],
	[101, $Roads/RoadPoint89],
	[102, $Roads/RoadPoint88],
	[103, $Roads/RoadPoint87],
	[104, $Roads/RoadPoint94],
	[105, $Roads/RoadPoint93],
	[106, $Roads/RoadPoint90],
	[107, $Roads/RoadPoint91],
	[108, $Roads/RoadPoint92],
	[271, $SeaPoints/SeaPoint70],
	[272, $SeaPoints/SeaPoint69],
	[273, $SeaPoints/SeaPoint68],
	[274, $SeaPoints/SeaPoint67],
	[109, $Roads/RoadPoint74],
	[110, $Roads/RoadPoint82],
	[111, $Roads/RoadPoint111],
	[251, $SeaPoints/SeaPoint28],
	[252, $SeaPoints/SeaPoint29],
	[253, $SeaPoints/SeaPoint30],
	[254, $SeaPoints/SeaPoint31],
	[255, $SeaPoints/SeaPoint32],
	[256, $SeaPoints/SeaPoint33],
	[257, $SeaPoints/SeaPoint34],
	[258, $SeaPoints/SeaPoint35],
	[259, $SeaPoints/SeaPoint36],
	[260, $SeaPoints/SeaPoint37],
	[261, $SeaPoints/SeaPoint38],
	[262, $SeaPoints/SeaPoint39],
	[263, $SeaPoints/SeaPoint40],
	[264, $SeaPoints/SeaPoint41],
	[265, $SeaPoints/SeaPoint42],
	[266, $SeaPoints/SeaPoint43],
	[267, $SeaPoints/SeaPoint44],
	[268, $SeaPoints/SeaPoint45],
	[269, $SeaPoints/SeaPoint46],
	[270, $SeaPoints/SeaPoint47]
]