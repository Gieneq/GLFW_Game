<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.10.0" name="objects_1x2" tilewidth="20" tileheight="40" tilecount="105" columns="15">
 <image source="objects_1x2.png" width="300" height="300"/>
 <tile id="0">
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="20" width="20" height="20"/>
  </objectgroup>
 </tile>
 <tile id="1">
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="20" width="20" height="20"/>
  </objectgroup>
 </tile>
 <tile id="2">
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="20" width="20" height="20"/>
  </objectgroup>
 </tile>
 <tile id="4">
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="20" width="20" height="20"/>
  </objectgroup>
 </tile>
 <tile id="6">
  <properties>
   <property name="active" type="bool" value="true"/>
   <property name="light" type="int" value="5"/>
   <property name="light_color" type="color" value="#ffe6c896"/>
   <property name="light_x" type="int" value="10"/>
   <property name="light_y" type="int" value="10"/>
   <property name="particles" value="flame"/>
   <property name="particles_x" type="int" value="10"/>
   <property name="particles_y" type="int" value="10"/>
  </properties>
  <objectgroup draworder="index" id="2">
   <object id="1" x="5" y="35" width="10" height="4"/>
  </objectgroup>
  <animation>
   <frame tileid="6" duration="100"/>
   <frame tileid="7" duration="100"/>
   <frame tileid="8" duration="100"/>
   <frame tileid="9" duration="100"/>
  </animation>
 </tile>
 <tile id="15">
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="20" width="20" height="20"/>
  </objectgroup>
  <animation>
   <frame tileid="15" duration="1000"/>
   <frame tileid="16" duration="1000"/>
   <frame tileid="17" duration="1000"/>
   <frame tileid="18" duration="1000"/>
  </animation>
 </tile>
 <tile id="19">
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="20" width="20" height="16"/>
  </objectgroup>
 </tile>
 <tile id="20">
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="20" width="20" height="16"/>
  </objectgroup>
 </tile>
 <tile id="30">
  <animation>
   <frame tileid="30" duration="1000"/>
   <frame tileid="31" duration="1000"/>
   <frame tileid="32" duration="1000"/>
   <frame tileid="33" duration="1000"/>
  </animation>
 </tile>
 <tile id="34">
  <properties>
   <property name="stairs_max" type="int" value="12"/>
   <property name="stairs_min" type="int" value="0"/>
  </properties>
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="0" width="20" height="40"/>
  </objectgroup>
 </tile>
 <tile id="35">
  <properties>
   <property name="stairs_max" type="int" value="20"/>
   <property name="stairs_min" type="int" value="12"/>
  </properties>
  <objectgroup draworder="index" id="3">
   <object id="2" x="0" y="0" width="20" height="20"/>
  </objectgroup>
 </tile>
</tileset>
