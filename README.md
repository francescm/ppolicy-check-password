ppolicy-check-password
======================

Password check modules with ppolicy overlay in openldap-2.4.30

Be sure your openldap is compiled with the --modules-enabled.

Create a password checker program in: 
$SOURCE/openldap-2.4.30/servers/slapd/overlays/

compile it:
#check_password:
	gcc -fPIC -c -I../../../include -I.. check_password.c
	gcc -shared -o check_password.so check_password.o

Create a openldap directory in the libexec dir where slapd is found, than copy check_password.so:
 sudo cp -v check_password.so $PREFIX/libexec/openldap/

Enable the overlay (see instruction at the end)

Create the ppolicy entry:
dn: ou=policies,dc=test,dc=com
ou: policies
objectClass: top
objectClass: organizationalUnit

dn: cn=default,ou=policies,dc=test,dc=com
cn: default
objectClass: pwdPolicy
objectClass: person
objectClass: top
pwdAllowUserChange: TRUE
pwdAttribute: userPassword
pwdExpireWarning: 600
pwdFailureCountInterval: 30
pwdGraceAuthNLimit: 5
pwdInHistory: 5
pwdLockout: TRUE
pwdLockoutDuration: 0
pwdMaxAge: 86400
pwdMaxFailure: 5
pwdMinAge: 0
pwdMinLength: 8
pwdMustChange: FALSE
pwdSafeModify: FALSE
sn: default value
objectClass: pwdPolicyChecker
pwdCheckQuality: 1
pwdCheckModule: check_password.so

please note: a ppolicy entry with enforces a pwdCheckModule should display a pwdPolicyChecker objectClass.

Check policies:
# sudo ldappasswd -e ppolicy -H ldapi:/// -Y EXTERNAL -s invalidpassword uid=user,ou=people,dc=test,dc=com
Result: Constraint violation (19)
control: 1.3.6.1.4.1.42.2.27.8.5.1 false MAOBAQU=
ppolicy: error=5 (Password fails quality checks)

In logs:
Sep  4 08:51:26 b1 slapd[25971]: conn=1000 op=1 PASSMOD id="uid=user,ou=people,dc=test,dc=com" new
Sep  4 08:51:26 b1 slapd[25971]: check_password_quality: module error: (check_password.so) 2: no digit.[1]
Sep  4 08:51:26 b1 slapd[25971]: conn=1000 op=1 RESULT oid= err=19 text=

Many thanks to:  Andris Eiduks  http://www.openldap.org/lists/openldap-software/200701/msg00382.html


Enable ppolicy with dynamic overlays
1) add module:
dn: cn=module{1},cn=config
objectClass: olcModuleList
cn: module{1}
olcModulePath: /usr/lib/ldap
olcModuleLoad: ppolicy

2) add schema (modify dn if you have less or more then 9 schema; change dn to first free number: cn={8}ppolicy,cn=schema,cn=config):

dn: cn={10}ppolicy,cn=schema,cn=config
objectClass: olcSchemaConfig
cn: {10}ppolicy
olcAttributeTypes: {0}( 1.3.6.1.4.1.42.2.27.8.1.1 NAME 'pwdAttribute' EQUALITY
  objectIdentifierMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.38 )
olcAttributeTypes: {1}( 1.3.6.1.4.1.42.2.27.8.1.2 NAME 'pwdMinAge' EQUALITY in
 tegerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )
olcAttributeTypes: {2}( 1.3.6.1.4.1.42.2.27.8.1.3 NAME 'pwdMaxAge' EQUALITY in
 tegerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )
olcAttributeTypes: {3}( 1.3.6.1.4.1.42.2.27.8.1.4 NAME 'pwdInHistory' EQUALITY
  integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )
olcAttributeTypes: {4}( 1.3.6.1.4.1.42.2.27.8.1.5 NAME 'pwdCheckQuality' EQUAL
 ITY integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )
olcAttributeTypes: {5}( 1.3.6.1.4.1.42.2.27.8.1.6 NAME 'pwdMinLength' EQUALITY
  integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )
olcAttributeTypes: {6}( 1.3.6.1.4.1.42.2.27.8.1.7 NAME 'pwdExpireWarning' EQUA
 LITY integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )
olcAttributeTypes: {7}( 1.3.6.1.4.1.42.2.27.8.1.8 NAME 'pwdGraceAuthNLimit' EQ
 UALITY integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )
olcAttributeTypes: {8}( 1.3.6.1.4.1.42.2.27.8.1.9 NAME 'pwdLockout' EQUALITY b
 ooleanMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.7 SINGLE-VALUE )
olcAttributeTypes: {9}( 1.3.6.1.4.1.42.2.27.8.1.10 NAME 'pwdLockoutDuration' E
 QUALITY integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )
olcAttributeTypes: {10}( 1.3.6.1.4.1.42.2.27.8.1.11 NAME 'pwdMaxFailure' EQUAL
 ITY integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE )
olcAttributeTypes: {11}( 1.3.6.1.4.1.42.2.27.8.1.12 NAME 'pwdFailureCountInter
 val' EQUALITY integerMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.27 SINGLE-VALUE 
 )
olcAttributeTypes: {12}( 1.3.6.1.4.1.42.2.27.8.1.13 NAME 'pwdMustChange' EQUAL
 ITY booleanMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.7 SINGLE-VALUE )
olcAttributeTypes: {13}( 1.3.6.1.4.1.42.2.27.8.1.14 NAME 'pwdAllowUserChange' 
 EQUALITY booleanMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.7 SINGLE-VALUE )
olcAttributeTypes: {14}( 1.3.6.1.4.1.42.2.27.8.1.15 NAME 'pwdSafeModify' EQUAL
 ITY booleanMatch SYNTAX 1.3.6.1.4.1.1466.115.121.1.7 SINGLE-VALUE )
olcAttributeTypes: {15}( 1.3.6.1.4.1.4754.1.99.1 NAME 'pwdCheckModule' DESC 'L
 oadable module that instantiates "check_password() function' EQUALITY caseExa
 ctIA5Match SYNTAX 1.3.6.1.4.1.1466.115.121.1.26 SINGLE-VALUE )
olcObjectClasses: {0}( 1.3.6.1.4.1.4754.2.99.1 NAME 'pwdPolicyChecker' SUP top
  AUXILIARY MAY pwdCheckModule )
olcObjectClasses: {1}( 1.3.6.1.4.1.42.2.27.8.2.1 NAME 'pwdPolicy' SUP top AUXI
 LIARY MUST pwdAttribute MAY ( pwdMinAge $ pwdMaxAge $ pwdInHistory $ pwdCheck
 Quality $ pwdMinLength $ pwdExpireWarning $ pwdGraceAuthNLimit $ pwdLockout $
  pwdLockoutDuration $ pwdMaxFailure $ pwdFailureCountInterval $ pwdMustChange
  $ pwdAllowUserChange $ pwdSafeModify ) )

3) Add overlay ppolicy:
[edit] Aggiungere lo overlay ppolicy

dn: olcOverlay=ppolicy,olcDatabase={2}hdb,cn=config
changetype: add
objectClass: olcOverlayConfig
objectClass: olcPPolicyConfig
olcOverlay: ppolicy
olcPPolicyDefault: cn=default,ou=policies,dc=unimore,dc=it
