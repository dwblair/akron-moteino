$fs = 0.5;
$fa = 1;

inner_r=10;
 wall_thickness=3;
 length= 140;
 
  screw_r=2.5;
 
 screw_distance=4;
 
 outer_r=inner_r+wall_thickness;

difference() {
    
    
  cylinder(h=length,r1=outer_r,r2=outer_r,center=true);
    
    
translate([0,0,-length*.65])
cylinder(h=length,r1=inner_r,r2=inner_r,center=false);

translate([0,0,length*.38])
cylinder(h=length*.15,r1=inner_r,r2=inner_r,center=false);

 translate([-screw_distance,0,0])
 cylinder(h=length*2, r1=screw_r, r2=screw_r, center=true);
 
 translate([screw_distance,0,0])
 cylinder(h=length*2, r1=screw_r, r2=screw_r, center=true);
 
 
 translate([0,0,length*.75])
 cube(size=[100,8,100],center=true);   
 
}


//difference() {
    
//cylinder(h=length,r1=outer_r,r2=outer_r,center=true);
    
//translate([0,0,-length*.75])
//cylinder(h=length,r1=inner_r,r2=inner_r,center=false);

//}
