# GeoMapProjector

This console application is a calculator of the coordinates of the points and regions between different map projections.  

It currently supports 16 different map projections: 5 azimuthal, 7 cylindrical, 2 conic and 2 hybrid.  

The application can calculate the distance between two desired points in any supported projection and calculates the area 
between latitudes and longitudes bordering a given region in any projection.  



## User guide
### Main features
 - conversion of the point with real geographical coordinates into the coordinates of the desired projection and vice versa
 - conversion of the region with real coordinates into the coordinates of the desired projection (including conversion of capital city's coordinates for countries)
 - calculation of the distance between two points or regions (see section **Data->Built-in data->Regions** for info about regions) in the desired projection
 - calculation of the area of a region (i.e. the area between the region's latitudinal and longitudinal borders) in the desired projection
 
 

### Projections
Supported projections:

**1. Azimuthal:**
- Gnomonic projection
- Stereographic projection
- Orthographic projection
- Postel projection
- Lambert (azimuthal equal-area) projection
     
**2. Cylindrical:**
 - tangent:
   - Marin (Equirectangular) projection
   - Lambert (cylindrical equal-area) projection
   - Mercator projection
   - Braun (Perspective) projection
 - secant:
   - Behrmann projection
   - Trystan-Edwards projection
   - Gall projection
          
**3. Conic:**
 - Ptolemy (Equidistant conic) projection
 - Lambert (conformal conic) projection
 

Source of the conversion formulae: *link to the slides if permitted*


### Data
#### Built-in data
The application has a built-in database containing:
1. **Points**
     - **cities** - at the start of the program, the user is asked whether the default number of the biggest cities in the world can be loaded or  the user wishes to change the number of cities being loaded into the database. The default number is 100 cities, but if needed, 26 570 cities can be loaded.  
      **NOTE:** If user requests bigger number of cities, only 26 570 cities will be loaded.
     - **mountains** - 61 largest mountains
 2. **Regions**:
     - **countries** - 173 countries, including the coordinates of the capital city. The capital city can be accessed via `<name_of_the_country>.capital` (e.g. `Slovakia.capital`).
     - **continents** - 6 continents: Africa, America, Asia, Australia, Europe and Oceania
     - **lakes** - 20 largest lakes   

    Regions are defined by their latitudinal and longitudinal borders. If the distance between the regions is being calculated, the region is represented as a point - the region's centroid.  
    **Longitudinal coordinate values are normalized** so their value is between -180 and 180 degrees (negative values on the western hemisphere and positive values on the eastern hemisphere).  
    **Latitudinal coordinate values are normalized** so their value is between -90 and 90 degrees in case of real coordinate system (negative values on southern hemisphere and positive values on northern hemisphere), and between 0 and 180 in case of polar coordinate system. 

#### Custom data
Custom points and regions can be added via `add [point|region] <name> <data>`. If custom data has real geographical coordinates (from now on called "real coordinates"), it is saved into the main database accessible from all projections. 

The custom point can also be saved with the coordinates of the current projection. In that case, it can be accessed only within that projection.  
For conversion of the local point to the real coordinates system (from now on called "real coordinates"), command `get point real <name>` can be used.

More about the commands in section **Commands**.


### Commands

#### Notation
For the commands description, the following rules are applied:
 - Brackets `[o1|o2|o3|...]` mean that the one of the options o1,o2,... must be chosen.
 - Angle brackets `<name>` mean that value with given property must be substituted in there. In this case, the argument is the name of the point/region.
    - when a `<name>` is in angle brackets, it can be a single word or multiple words bounded with the quotation marks, e.g. `Slovakia`,`"Hong Kong"`, `"Czech Republic.capital"`
 - Parentheses `()` mean that their content is an optional parameter.


#### Supported commands

- `help [enter|add|home|print|get|projections|<projection_type>]`
   - prints a manual page for a given category, its content is very similar to the description of the commands in this section. Typing plane `help` prints the possible manual pages.
   - `help projections` and `help <projection_type>` prints the additional information about projection types and subtypes, the rest of the manual pages prints the information about other supported commands.
   - example: `help print`,`help azimuthal`

- `enter <main_projection_type> <projection_subtype>` - changes the current projection into the given one. Projection type and subtype must be written in lowercase.
  - `<main_projection_type>` is one of the listed in `help projections` manual page
  - `<projection_subtype>` - one of the listed in `help <main_projection_type>`
  - example: `enter conic ptolemy`, `enter hybrid sanson`

- `add [(1)|(2)]` - adds the given data to the main or local database.
   - (1) `point [real|local] <name> <lat> <lon>` - saves the point with name `<name>` and coordinates `<lat> <lon>`
   	   - `local <name> <lat> <lon>` - the point is saved into the local projection's
   	   database with the projection's coordinates having the orientation of the latitude and longitude in real geographical projection.
   	   - `real <name> <lat> <lon>` - the point is saved into the main database in section "custom".
   	   	   - `<lat> <lon>` are the latitude and longitude in a real geographical projection.
   - (2) `region <name> <south> <north> <east> <west>` - saves the region with name `<name>` and the latitudinal and longitudinal borders in real geographical coordinates. The region is saved into the main database in section "custom".

   - example: 
      - `add point local "My new local point" 15.3 60` - saves "My new local point" into the current projection's database. Considering the current projection is azimuthal, the coordinates are polar with values rho=15.3 and epsilon=60.
      - `add region reg -20 30 60.1 2.123` - saves the region named "reg" with southern border 20°S, northern border 30°N, eastern border 60.1°E and western border 2.123°E into the main database.

- `home` - changes the current projection back into the real geographical coordinate system.

- `print [countries|cities|mountains|lakes|continents|custom|local]` - prints the given data that is saved in that category in the database.
  - `print local` - prints the custom local points saved in the current projection in local coordinate system.
  - the rest of the options prints data saved in the main database.  
  **NOTE:** `print cities` prints all the cities in the database, so the user should be careful with defining the custom number of cities being loaded into the database at the start of the program.

- `get [(1)|(2)|(3)|(4)]` - prints the results of a chosen coordinate system conversion, distance between points or the area of a region. 
   - (1) `point (real) <name>` 
   	   - when `real` is present, it prints the real coordinates of a point. It can be used to transform coordinates of the locally saved point into the real coordinates.
       - when `real` is **not** present, it prints the coordinates of a point in the current projection.
   - (2) `region <name>` - prints the coordinates of a region in the current projection. If the region is a country, it also prints the coordinates of the capital city.
   - (3) `distance <name1> <name2> (scale <scale> (units [m|cm|mm]))` - prints the distance between two points. The result is the distance within the current projection, by default in kilometres.
   	   - by adding `scale <scale>` the scale of the projection can be set.
   	   - `<scale>` has to be in format `1:<real_number>`.
   	   - when using `scale <scale>`, the units can be set by adding `units [m|cm|mm]`.
   - (4) `area <name> (scale <scale> (units [m|cm|mm]))` - prints the area between the longitudes and the latitudes defining the borders of the region `<name>` in kilometres squared.
   	   - as in case (3), scale and units can be set, with resulting units being in `[m|cm|mm]` squared.

- `exit` - terminates the program. The custom data is deleted.


## Developer documentation

The program is implemented in C++20.  
The initial data of the database are stored as csv files.  

### Classes

- **db_loader** - loads the initial data from csv files into the database.
- **Database** - a class storing the data as `map<string, shared_ptr<Data>>` for each category. The categories are: `countries`,`lakes`,`cities`,`mountains`,`continents`,`custom_points`,`custom_regions` and `data` (stores everything in a single map). This class also prints the data saved in a given category. 
  The database contains pointers to **Data**, which is a base class for classes:
     - **Point** - stores points as `shared_ptr<Coords>`
     - **Region** - stores regions and countries. It contains `shared_ptr<Coords>` for each region's border, the region's centroid, and if the region is a country, also the capital (in this case it also stores the capital name as `string`).
- **Coords** - the base class for each type of coordinates. The coordinates are stored in `pair<double,double>`. The regions' borders can be represented as a single `double` value, so `enum singleton_type = {LATITUDE,LONGITUDE,BOTH}` is set while initialization. The data is then read according to their `singleton_type`.  
Derived classes:
   - **RealCoords** - the coordinates of a real geographical projection. Besides `pair<double,double>` of the base class, the data is also saved in `double latitude` and `double longitude` for more intuitive access to the values.
   - **PolarCoords** - the coordinates of polar coordinate system used in azimuthal, conic and some of the hybrid projections. The coordinates are also saved in `double rho` and `double epsilon`.
   - **CartesianCoords** - the coordinates of the cartesian coordinate system used in azimuthal, and some of the hybrid projections. The coordinates are also saved in `double y` and `double x`.  
- **Projections** - class uniting classes for each supported projection. Directly derived classes are:
    - **RealProjection** - supports computing with real geographical coordinate system, i.e. **RealCoords**.
    - **AzimuthalProjection** - uses **PolarCoords**, it is a base class for all supported azimuthal subtypes and a hybrid - Werner-Stab projection.
    - **CylindricalProjection** - uses **CartesianCoords**, it is a base class for all supported cylindrical subtypes and a hybrid - Sanson projection.
    - **ConicProjection** - uses **PolarCoords**, it is a base class for all supported conic projections.
- **Controller** - a class processing the user's input and printing the corresponding output. It contains all manual pages as `string`s and prints error messages. 


### Calculations
- Distance and area calculation is done using template `get distance(T &p0, T &p1)` and `get_rectangular_area(T &s, T &n, T &e, T &w)` for each derived class of **Coords**. 
- `my_math.cpp` contains functions for alternatives of goniometric functions from `cmath` library, taking arguments in degrees instead of radians.
- Conversions between map projections are defined in each subclass of **Projection**'s derived classes.
- Scaling and conversion between units is done directly in **Controller**'s method `get_cmd`.
