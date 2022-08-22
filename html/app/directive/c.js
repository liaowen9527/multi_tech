(function (app) {
    'use strict';

    angular.module('nb').directive('nbCDirective', nbCDirective);
    
    nbCDirective.$inject = [];
    function nbCDirective() {
        var directive = {
            restrict: 'EA',
            templateUrl: 'app/html/c.html',
            link: linkFunc,
            controller: 'nbCCtrl',
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