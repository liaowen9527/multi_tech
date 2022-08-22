(function (app) {
    'use strict';

    angular.module('nb').directive('nbADirective', nbADirective);
    
    nbADirective.$inject = [];
    function nbADirective() {
        var directive = {
            restrict: 'EA',
            templateUrl: 'app/html/a.html',
            link: linkFunc,
            controller: 'nbACtrl',
            controllerAs: 'inst',
            scope: {
                data: "="
            }
        };

        return directive;
        function linkFunc(scope, ele, attr, ctrl) {
            ctrl.firstName = scope.data.firstName;
            ctrl.lastName = scope.data.lastName;
        }
    }

})(NetBrainApp);